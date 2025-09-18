#include "AIP1640_LED_Matrix.h"

// The constructor now initializes the pin variables.
AIP1640_LED_Matrix::AIP1640_LED_Matrix(int clkPin, int dioPin) {
    _clkPin = clkPin;
    _dioPin = dioPin;
}

void AIP1640_LED_Matrix::begin() {
    pinMode(_dioPin, OUTPUT);
    pinMode(_clkPin, OUTPUT);
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
}

void AIP1640_LED_Matrix::i2cStart() {
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(3);
    digitalWrite(_dioPin, HIGH);
    delayMicroseconds(3);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(3);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(3);
}

void AIP1640_LED_Matrix::i2cSend(unsigned char data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(_clkPin, LOW);
        delayMicroseconds(3);
        if (data & 0x01) {
            digitalWrite(_dioPin, HIGH);
        } else {
            digitalWrite(_dioPin, LOW);
        }
        delayMicroseconds(3);
        digitalWrite(_clkPin, HIGH);
        delayMicroseconds(3);
        data = data >> 1;
    }
}

void AIP1640_LED_Matrix::i2cEnd() {
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(3);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(3);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(3);
    digitalWrite(_dioPin, HIGH);
    delayMicroseconds(3);
}

void AIP1640_LED_Matrix::clear() {
    for (int i = 0; i < 16; i++) {
        displayBuffer[i] = 0;
    }
}

void AIP1640_LED_Matrix::setPixel(int x, int y, bool state) {
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
            if (pattern[row * width + col] == 1) {
                setPixel(col + xOffset, row + yOffset, true);
            }
        }
    }
}

void AIP1640_LED_Matrix::update() {
    i2cStart();
    i2cSend(0x40);
    i2cEnd();
    i2cStart();
    i2cSend(0xC0);
    for (int i = 0; i < 16; i++) {
        i2cSend(displayBuffer[i]);
    }
    i2cEnd();
    i2cStart();
    i2cSend(0x8A);
    i2cEnd();
}
