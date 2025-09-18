#ifndef AIP1640_LED_MATRIX_H
#define AIP1640_LED_MATRIX_H

#include <Arduino.h>

class AIP1640_LED_Matrix {
public:
    // A new constructor to accept the clock (CLK) and data (DIO) pins.
    AIP1640_LED_Matrix(int clkPin, int dioPin);
    
    void begin();
    void clear();
    void update();
    void setPixel(int x, int y, bool state);
    void drawPattern(const byte *pattern, int xOffset, int yOffset, int width, int height);

private:
    void i2cStart();
    void i2cSend(unsigned char data);
    void i2cEnd();
    
    // Private variables to store the pin numbers.
    int _clkPin;
    int _dioPin;
    
    byte displayBuffer[16];
};

#endif
