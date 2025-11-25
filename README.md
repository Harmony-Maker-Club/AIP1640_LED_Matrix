# AIP1640_LED_Matrix

A simple and flexible Arduino library for controlling a 16x8 LED matrix display based on the AIP1640 chip.

---

### Key Features
* **Flexible Pin Connection:** Use any two available digital pins on your Arduino board for communication.
* **Repurposes the AiP1640's non-conforming I2C connections:** This library uses a custom "bit-banging" communication protocol to interface with the AIP1640, as the chip does not conform to the standard I2C protocol. This enables the board to be connected to any two Arduino pins.
* **Brightness Control:** Adjust the display brightness (0-7) on the fly.
* **Pixel-Level Control:** Easily turn individual pixels on or off with `setPixel()`.
* **Pattern Drawing:** A convenient `drawPattern()` function for displaying custom shapes and characters.
* **Efficient:** Uses a display buffer to build patterns in memory before writing them to the display.

---

### Installation
You can install this library through the **Arduino Library Manager** in the IDE, or by manually downloading the `.zip` file from the [releases page](https://github.com/Harmony-Maker-Club/AIP1640_LED_Matrix/releases).

---

### Getting Started
Connect your AIP1640 LED matrix to your Arduino board. While the display labels its pins as SCL and SDA, you can connect them to any two digital pins on your board.

- **Display's SCL pin** → **Arduino digital pin** (e.g., pin 8)
- **Display's SDA pin** → **Arduino digital pin** (e.g., pin 9)

### Basic Usage Example

This example demonstrates how to set up the matrix, turn on a single pixel, and update the display.

```cpp
#include <AIP1640_LED_Matrix.h>

// Define the pins you are using
const int CLK_PIN = 8; 
const int DIO_PIN = 9; 

AIP1640_LED_Matrix matrix(CLK_PIN, DIO_PIN);

void setup() {
  matrix.begin();
  
  // Set brightness (0 = dim, 7 = brightest)
  matrix.setBrightness(7); 
  
  matrix.clear();
}

void loop() {
  // Turn on a pixel at column 8, row 4
  matrix.setPixel(8, 4, true);

  // Update the display to show the changes
  matrix.update();

  delay(1000);
  
  matrix.clear();
  matrix.update();
  
  delay(1000);
}
```

### API Reference

#### `AIP1640_LED_Matrix(clkPin, dioPin)`
Constructor. Initializes the library with the specified pins.

#### `void begin()`
Sets up the pins for output. Call this in `setup()`.

#### `void setBrightness(uint8_t level)`
Sets the brightness of the display.
* `level`: A value from `0` (dimmest/off) to `7` (brightest).

#### `void setPixel(int x, int y, bool state)`
Sets the state of a single pixel in the buffer.
* `x`: Column (0-15)
* `y`: Row (0-7)
* `state`: `true` for ON, `false` for OFF.

#### `void drawPattern(const byte *pattern, int xOffset, int yOffset, int width, int height)`
Draws a bitmap pattern into the buffer at a specific location.

#### `void clear()`
Clears the display buffer (sets all pixels to off).

#### `void update()`
Sends the current buffer content to the physical display. You must call this to see any changes!

---
## License

This project is licensed under the **MIT License** - see the `LICENSE.md` file for details.
