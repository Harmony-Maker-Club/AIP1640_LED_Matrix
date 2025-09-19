# AIP1640_LED_Matrix

A simple and flexible Arduino library for controlling a 16x8 LED matrix display based on the AIP1640 chip.

---

### Key Features
* **Flexible Pin Connection:** Use any two available digital pins on your Arduino board for communication.
* **Repurposes the AiP1640's non-conforming I2C connections:** This library uses a custom "bit-banging" communication protocol to interface with the AIP1640, as the chip does not conform to the standard I2C protocol. This enables the board to be connected to any two Arduino pins.
* **Pixel-Level Control:** Easily turn individual pixels on or off with `setPixel()`.
* **Pattern Drawing:** A convenient `drawPattern()` function for displaying custom shapes and characters.
* **Display Buffer:** The library uses a display buffer, allowing you to build complex patterns in memory before writing them to the physical display using `update()`.

---

### Installation
You can install this library through the **Arduino Library Manager** in the IDE, or by manually downloading the `.zip` file from the [releases page](https://github.com/Harmony-Maker-Club/AIP1640_LED_Matrix/releases).

---

### Getting Started
Connect your AIP1640 LED matrix to your Arduino board. While the display labels its pins as SCL and SDA, you can connect them to any two digital pins on your board.

- **Display's SCL pin** → **Arduino digital pin** (e.g., pin 2)
- **Display's SDA pin** → **Arduino digital pin** (e.g., pin 3)

### Basic Usage Example

This example demonstrates how to set up the matrix, turn on a single pixel, and update the display.

```cpp
#include <AIP1640_LED_Matrix.h>

// Note: While the display hardware labels these pins as SCL and SDA,
// the AIP1640 chip does not conform to the I2C protocol. This library
// uses a custom "bit-banging" algorithm, allowing it to interface with the
// display on any available digital pins, thereby freeing up the dedicated
// hardware I2C lines for other devices that require them.
const int CLK_PIN = 2; // Connect to the SCL pin on the display
const int DIO_PIN = 3; // Connect to the SDA pin on the display

AIP1640_LED_Matrix matrix(CLK_PIN, DIO_PIN);

void setup() {
  matrix.begin();
  matrix.clear();
}

void loop() {
  // Turn on a pixel at (8, 4)
  matrix.setPixel(8, 4, true);

  // Update the display to show the changes
  matrix.update();

  delay(1000);
  
  matrix.clear();
  matrix.update();
  
  delay(1000);
}
```

# Contributing
---
We welcome contributions from the community! If you'd like to contribute, please follow these steps:

1.  **Fork the repository.**
2.  **Create a new branch** for your feature (`git checkout -b feature/your-feature-name`).
3.  **Commit your changes** (`git commit -am 'Add new feature'`).
4.  **Push to the branch** (`git push origin feature/your-feature-name`).
5.  **Create a new Pull Request.**

---
## License

This project is licensed under the **MIT License** - see the `LICENSE.md` file for details.
