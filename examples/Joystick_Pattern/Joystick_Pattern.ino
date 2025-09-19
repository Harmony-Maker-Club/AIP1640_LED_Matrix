#include <AIP1640_LED_Matrix.h>

// Create a new instance of our custom library class to control the display.
// The first number is the CLK pin (SCL) and the second is the DIO pin (SDA).
// You can change these to match your wiring!
const int sclPin = 8;
const int sdaPin = 9;
AIP1640_LED_Matrix myMatrix(sclPin, sdaPin);

// Variables to track the position of the pattern
int xPos = 0;
int yPos = 0;

// Variables for joystick readings and movement
const int joyXpin = A0;
const int joyYpin = A1;
const int joyThreshold = 100;
const int moveInterval = 50; // Milliseconds to wait between moves
unsigned long lastMoveTime = 0; // The last time the pattern was moved

// --- Variables for button debouncing and pattern cycling ---
const int changePatternButtonPin = 2; // Digital pin for the button
int lastButtonState = HIGH;            // The previous reliable reading from the input pin
unsigned long lastDebounceTime = 0;    // The last time the button was pressed
unsigned long debounceDelay = 300;     // Wait for 300 ms to ensure a stable press

int currentPatternIndex = 0; // Index of the currently displayed pattern

// This is our data structure to hold pattern information.
// It bundles a pattern, its width, and its height together.
struct PatternInfo {
 const byte* pattern;
 int width;
 int height;
};

// Now we can define our patterns using 2D arrays. This is the part of the code
// that is perfect for teaching students about how a 2D grid of data can be
// represented in code.
const byte heart[5][5] = {
 {0, 1, 0, 1, 0},
 {1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1},
 {0, 1, 1, 1, 0},
 {0, 0, 1, 0, 0}
};

const byte simpleFace[4][4] = {
{0, 1, 1, 0},
{0, 0, 0, 0},
{1, 0, 0, 1},
{0, 1, 1, 0}
};

const byte triangle[2][3] = {
 {0, 1, 0},
 {1, 1, 1}
};

// An array of our PatternInfo structs.
const PatternInfo allPatterns[] = {
   { (const byte *)heart, 5, 5 },
   { (const byte *)simpleFace, 4, 4 },
   { (const byte *)triangle, 3, 2 }
};
const int numPatterns = sizeof(allPatterns) / sizeof(allPatterns[0]);

void setup() {
  myMatrix.begin();
  // We use INPUT_PULLUP, which enables an internal resistor to hold the pin HIGH
  // This means the button press will pull the pin LOW, which is a very reliable signal.
  pinMode(changePatternButtonPin, INPUT_PULLUP);
}

// Function to handle joystick movement for a pattern
void handleJoystick(int patternWidth, int patternHeight) {
  int joyX = analogRead(joyXpin);
  int joyY = analogRead(joyYpin);

  // Check for horizontal movement
  if (joyX < (512 - joyThreshold)) {
    if (xPos > 0) {
      xPos--;
    }
  } else if (joyX > (512 + joyThreshold)) {
    if (xPos < 16 - patternWidth) {
      xPos++;
    }
  }

  // Check for vertical movement
  if (joyY < (512 - joyThreshold)) {
    if (yPos > 0) {
      yPos--;
    }
  } else if (joyY > (512 + joyThreshold)) {
    if (yPos < 8 - patternHeight) {
      yPos++;
    }
  }
}

void loop() {
  // Read the current state of the button
  int reading = digitalRead(changePatternButtonPin);
    // This is the key change. We check if the button is currently pressed (LOW)
  // AND if enough time has passed since the last known press.
  if (reading == LOW && (millis() - lastDebounceTime > debounceDelay)) {
    // This is a valid, debounced button press.
    currentPatternIndex = (currentPatternIndex + 1) % numPatterns;
    
    // Reset position to center for the new pattern
    const PatternInfo& currentPattern = allPatterns[currentPatternIndex];
    xPos = (16 - currentPattern.width) / 2;
    yPos = (8 - currentPattern.height) / 2;

    // Reset the debounce timer to prevent another change from this same press
    lastDebounceTime = millis();
  }
    // Only update the display if enough time has passed.
  if (millis() - lastMoveTime > moveInterval) {
    // Get the current pattern and its dimensions from our array
    const PatternInfo& currentPattern = allPatterns[currentPatternIndex];
    
    handleJoystick(currentPattern.width, currentPattern.height);
    
    myMatrix.clear();
    
    // Draw the current pattern using the data from the struct
    myMatrix.drawPattern(currentPattern.pattern, xPos, yPos, currentPattern.width, currentPattern.height);
    
    myMatrix.update();

    // Reset the timer for the next update
    lastMoveTime = millis();
  }
}
