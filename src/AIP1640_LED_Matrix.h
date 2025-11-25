#ifndef AIP1640_LED_MATRIX_H
#define AIP1640_LED_MATRIX_H

#include <Arduino.h>

class AIP1640_LED_Matrix {
public:
    AIP1640_LED_Matrix(uint8_t clkPin, uint8_t dioPin);
    
    void begin();
    void clear();
    // Brightness level: 0 (off) to 7 (brightest)
    void setBrightness(uint8_t level); 
    void update();
    void setPixel(int x, int y, bool state);
    void drawPattern(const byte *pattern, int xOffset, int yOffset, int width, int height);

private:
    void startSignal();
    void sendByte(uint8_t data);
    void stopSignal();
    
    uint8_t _clkPin;
    uint8_t _dioPin;
    uint8_t _brightness; // Store current brightness
    
    // 16 bytes for 16 columns (assuming 8 rows high)
    uint8_t displayBuffer[16];
};

#endif
