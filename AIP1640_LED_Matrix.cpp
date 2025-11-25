#include "AIP1640_LED_Matrix.h"

AIP1640_LED_Matrix::AIP1640_LED_Matrix(uint8_t clkPin, uint8_t dioPin) {
    _clkPin = clkPin;
    _dioPin = dioPin;
    _brightness = 0x03; // Default mid-brightness
}

void AIP1640_LED_Matrix::begin() {
    pinMode(_dioPin, OUTPUT);
    pinMode(_clkPin, OUTPUT);
    
    // Idle state for I2C-like protocols is usually High
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
}

void AIP1640_LED_Matrix::startSignal() {
    // Start Condition: DIO falls while CLK is HIGH
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, LOW); // Prepare for data change
}

void AIP1640_LED_Matrix::sendByte(uint8_t data) {
    // LSB First
    for (int i = 0; i < 8; i++) {
        digitalWrite(_clkPin, LOW);
        
        // Set data bit
        if (data & 0x01) {
            digitalWrite(_dioPin, HIGH);
        } else {
            digitalWrite(_dioPin, LOW);
        }
        delayMicroseconds(1); 
        
        // Clock High (Chip reads data here)
        digitalWrite(_clkPin, HIGH);
        delayMicroseconds(1);
        
        data >>= 1;
    }
    // Return clock to low after byte
    digitalWrite(_clkPin, LOW);
}

void AIP1640_LED_Matrix::stopSignal() {
    // Stop Condition: DIO rises while CLK is HIGH
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, HIGH);
}

void AIP1640_LED_Matrix::clear() {
    // memset is faster than a for loop
    memset(displayBuffer, 0, 16);
}

void AIP1640_LED_Matrix::setBrightness(uint8_t level) {
    if (level > 7) level = 7;
    _brightness = level;
}

void AIP1640_LED_Matrix::setPixel(int x, int y, bool state) {
    // x = column (0-15), y = row (0-7)
    if (x < 0 || x > 15 || y < 0 || y > 7) {
        return;
    }
    if (state) {
        displayBuffer[x] |= (1 << y);
    } else {
        displayBuffer[x] &= ~(1 << y);
    }
}

void AIP1640_LED_Matrix::drawPattern(const byte *pattern, int xOffset, int yOffset, int width, int height) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Calculate absolute position
            int targetX = col + xOffset;
            int targetY = row + yOffset;

            // Bounds check before setting
            if (targetX >= 0 && targetX <= 15 && targetY >= 0 && targetY <= 7) {
                 // Assuming pattern is stored row-wise
                 if (pattern[row * width + col] == 1) {
                    setPixel(targetX, targetY, true);
                }
            }
        }
    }
}

void AIP1640_LED_Matrix::update() {
    // Command 1: Set Data Mode (Auto increment)
    startSignal();
    sendByte(0x40);
    stopSignal();

    // Command 2: Set Address (Start at 0x00)
    startSignal();
    sendByte(0xC0);
    
    // Send the buffer
    for (int i = 0; i < 16; i++) {
        sendByte(displayBuffer[i]);
    }
    stopSignal();

    // Command 3: Display Control
    // 0x80 is base command. 
    // 0x88 adds the "Display ON" bit.
    // Lower 3 bits are pulse width (brightness).
    startSignal();
    sendByte(0x88 | _brightness); 
    stopSignal();
}
