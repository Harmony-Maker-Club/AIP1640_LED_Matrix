// This is the PONG game sketch for the AIP1640 LED matrix.
#include <AIP1640_LED_Matrix.h>

// --- Game Constants and Pins ---
#define PLAYER1_JOYSTICK_PIN A0
#define PLAYER2_JOYSTICK_PIN A1
#define MODE_BUTTON_PIN 2

#define PADDLE_SIZE 3
#define BALL_SIZE 1
#define X_MAX 15 // The maximum x-coordinate (column 15)
#define Y_MAX 7
#define PADDLE_WIDTH 1
#define WINNING_SCORE 9

// --- Library Setup ---
// The constructor now requires the clock (CLK) and data (DIO) pins.
AIP1640_LED_Matrix myMatrix(8, 9);

// --- Game State Variables ---
int paddle1Y;
int paddle2Y;
float ballX;
float ballY;
float ballVelX;
float ballVelY;
int score1;
int score2;
int gameMode = 0; // 0 for title screen, 1 for game, 2 for end screen
unsigned long lastButtonPress = 0;
const unsigned long BUTTON_DEBOUNCE_TIME = 200;
unsigned long lastGameUpdate = 0;
const unsigned long GAME_UPDATE_DELAY = 100;

// Corrected 3x5 patterns for each digit (15 elements)
const byte numberPatterns[10][15] = {
  {0,1,0, 1,0,1, 1,0,1, 1,0,1, 0,1,0}, // 0
  {0,1,0, 1,1,0, 0,1,0, 0,1,0, 1,1,1}, // 1
  {1,1,1, 0,0,1, 1,1,1, 1,0,0, 1,1,1}, // 2
  {1,1,1, 0,0,1, 1,1,1, 0,0,1, 1,1,1}, // 3
  {1,0,1, 1,0,1, 1,1,1, 0,0,1, 0,0,1}, // 4
  {1,1,1, 1,0,0, 1,1,1, 0,0,1, 1,1,1}, // 5
  {1,1,1, 1,0,0, 1,1,1, 1,0,1, 1,1,1}, // 6
  {1,1,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1}, // 7
  {1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,1,1}, // 8
  {1,1,1, 1,0,1, 1,1,1, 0,0,1, 1,1,1}  // 9
};

// --- Function to draw a number on the matrix ---
void drawNumber(int number, int xOffset) {
  if (number < 0 || number > 9) return;
  myMatrix.drawPattern(numberPatterns[number], xOffset, 1, 3, 5); // Using 3x5 size
}

// --- Game Logic Functions ---
void resetBall() {
  // Reset paddle and ball positions
  paddle1Y = 3;
  paddle2Y = 3;
  ballX = X_MAX / 2;
  ballY = Y_MAX / 2;
  ballVelX = 1.0;
  ballVelY = 1.0;
  // Randomize initial ball direction
  randomSeed(analogRead(A0));
  if (random(2)) ballVelX *= -1.0;
  if (random(2)) ballVelY *= -1.0;
  }

  void startNewGame() {
  // Reset scores and call resetBall to set initial positions
  score1 = 0;
  score2 = 0;
  resetBall();
}

void updateGame() {
  // Read joystick values and update paddle positions
  int joy1 = analogRead(PLAYER1_JOYSTICK_PIN);
  int joy2 = analogRead(PLAYER2_JOYSTICK_PIN);

  // Pushing the joystick up (low value) moves the paddle up.
  if (joy1 < 200 && paddle1Y > 0) {
    paddle1Y--;
  } else if (joy1 > 800 && paddle1Y < Y_MAX - PADDLE_SIZE + 1) {
    paddle1Y++;
  }

  // Pushing the joystick up (low value) moves the paddle up.
  if (joy2 < 200 && paddle2Y > 0) {
    paddle2Y--;
  } else if (joy2 > 800 && paddle2Y < Y_MAX - PADDLE_SIZE + 1) {
    paddle2Y++;
  }

  // Update ball position
  ballX += ballVelX;
  ballY += ballVelY;

  // Collision with top/bottom walls
  if (ballY <= 0 || ballY >= Y_MAX) {
    ballVelY *= -1;
  }

  // Collision with paddles
  if ((ballX <= PADDLE_WIDTH && ballY >= paddle1Y && ballY < paddle1Y + PADDLE_SIZE) ||
      (ballX >= X_MAX - PADDLE_WIDTH && ballY >= paddle2Y && ballY < paddle2Y + PADDLE_SIZE)) {
    ballVelX *= -1;
  }

  // Scoring
  if (ballX < 0) {
    score2++;
    resetBall();
  } else if (ballX > X_MAX) {
    score1++;
    resetBall();
  }
  // Check for win condition
  if (score1 >= WINNING_SCORE || score2 >= WINNING_SCORE) {
    gameMode = 2; // Transition to end screen
  }
}

void drawGame() {
  myMatrix.clear();
  // Draw paddles
  for (int i = 0; i < PADDLE_SIZE; i++) {
    myMatrix.setPixel(0, paddle1Y + i, true);
    // Corrected paddle position: Draw on the last column (X_MAX)
    myMatrix.setPixel(X_MAX, paddle2Y + i, true);
  }

  // Draw ball
  myMatrix.setPixel(static_cast<int>(ballX), static_cast<int>(ballY), true);
  // Draw scores
  drawNumber(score1, 5);
  drawNumber(score2, 9);
  myMatrix.update();
}


void displayTitle() {
  myMatrix.clear();
  // Corrected 3x5 patterns for 'PONG' (15 elements each) to fit the display
  const byte pPattern[15] = {1,1,0, 1,0,1, 1,1,0, 1,0,0, 1,0,0};
  const byte oPattern[15] = {0,1,0, 1,0,1, 1,0,1, 1,0,1, 0,1,0};
  // The new pattern for 'N' that looks like an lowercase 'n'
  const byte nPattern[15] = {0,0,0, 1,1,0, 1,0,1, 1,0,1, 1,0,1};
  const byte gPattern[15] = {1,1,1, 1,0,0, 1,0,1, 1,0,1, 1,1,1};

  // Draw 'PONG' with 3x5 dimensions and a 1-pixel space between letters
  myMatrix.drawPattern(pPattern, 0, 1, 3, 5);
  myMatrix.drawPattern(oPattern, 4, 1, 3, 5);
  myMatrix.drawPattern(nPattern, 8, 1, 3, 5);
  myMatrix.drawPattern(gPattern, 12, 1, 3, 5);

  myMatrix.update();
}

void displayScore() {
  myMatrix.clear();
  drawNumber(score1, 5);
  drawNumber(score2, 9);
  myMatrix.update();
}

void setup() {
  myMatrix.begin();
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  startNewGame();
}

void loop() {
  // Handle button press to toggle game modes
  if (digitalRead(MODE_BUTTON_PIN) == LOW && millis() - lastButtonPress > BUTTON_DEBOUNCE_TIME) {
    if (gameMode == 0) {
      // From title screen, start a new game
      startNewGame();
      gameMode = 1;
    } else if (gameMode == 2) {
      // From end screen, go back to title screen
      gameMode = 0;
    }
    lastButtonPress = millis();
    myMatrix.clear();
    myMatrix.update(); // Update display to clear it immediately
  }

  // State machine for different modes
  if (gameMode == 0) {
    // Display Title Screen
    displayTitle();
  } else if (gameMode == 1) {
    // Play the game
    if (millis() - lastGameUpdate > GAME_UPDATE_DELAY) {
      updateGame();
      drawGame();
      lastGameUpdate = millis();
    }
  } else if (gameMode == 2) {
    // Display end-game scores
    displayScore();
  }
}
