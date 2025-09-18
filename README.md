# AIP1640 LED Matrix Library

An Arduino library for controlling 8x16 LED matrices driven by the AIP1640 chip. This library simplifies displaying text, patterns, and pixels on the matrix display. It uses a simple bit-banging protocol, which means it doesn't require any special hardware other than the LED matrix itself and your Arduino board.

## Features

-   **Simple API**: Easily draw pixels, lines, rectangles, and text.
-   **Text Display**: Built-in functionality for scrolling text on the matrix.
-   **Low Overhead**: The library is lightweight and designed to be efficient for use on microcontrollers.
-   **Bit-Banging Protocol**: No dedicated I2C or SPI hardware is required, freeing up those pins for other uses.

## Getting Started

### Prerequisites

* An Arduino board (e.g., Uno, Nano, or ESP32)
* An 8x16 LED Matrix with an AIP1640 driver chip
* Jumper wires to connect the matrix to your Arduino

### Installation

1.  **Download the library**: Click the "Code" button on the GitHub repository page, and select "Download ZIP".
2.  **Add to Arduino IDE**: Open your Arduino IDE, go to `Sketch` -> `Include Library` -> `Add .ZIP Library`, and select the downloaded file.

### Basic Usage

Here's a simple example to get you started:

```cpp
#include <AIP1640.h>

// Define the pins connected to your matrix (CLK, DIO)
const int CLK_PIN = 2;
const int DIO_PIN = 3;

AIP1640 matrix(CLK_PIN, DIO_PIN);

void setup() {
  // Initialize the matrix
  matrix.begin();
  // Clear the display
  matrix.clearDisplay();
  // Set the brightness (0-7)
  matrix.setBrightness(4);
}

void loop() {
  // Draw a single pixel at (x=0, y=0)
  matrix.setPixel(0, 0, true);
  // Update the display to show the pixel
  matrix.updateDisplay();
  delay(1000);
  
  // Clear the pixel
  matrix.setPixel(0, 0, false);
  matrix.updateDisplay();
  delay(1000);
}
