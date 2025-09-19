#include <AIP1640_LED_Matrix.h>

// Initialize the matrix with clock and data pins
AIP1640_LED_Matrix matrix(2, 3);

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
