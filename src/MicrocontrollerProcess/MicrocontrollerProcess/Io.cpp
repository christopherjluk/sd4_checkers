/************************************************************
 * @file Io.cpp
 * @brief The implementation for I/O related functionalities
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"
#include "Io.h"
#include "VoiceRecognition.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "Arduino.h"
#include "LedControl.h"

/**********************************
 ** Defines
 **********************************/
/* Pins */
#define BUTTON_ARRAY_PIN1              (36)
#define BUTTON_ARRAY_PIN2              (39)
#define BUTTON_ARRAY_PIN3              (34)
#define BUTTON_ARRAY_PIN4              (35)
#define BUTTON_POWER_PIN               (32)
#define PLAYER1_TURN_INDICATOR_LED_PIN (12)
#define PLAYER2_TURN_INDICATOR_LED_PIN (13)
#define LED_MAX_CHIP_RED_PIN           (33)
#define LED_MAX_CHIP_GREEN_PIN         (25)
#define LED_MAX_CHIP_BLUE_PIN          (26)
#define LED_MAX_CHIP_CS_PIN            (27)
#define LED_MAX_CHIP_CLK_PIN           (14)

/* Button array thresholds */
#define BUTTON_THRESHOLD1 (40)
#define BUTTON_THRESHOLD2 (250)
#define BUTTON_THRESHOLD3 (800)
#define BUTTON_THRESHOLD4 (1400)
#define BUTTON_THRESHOLD5 (1850)
#define BUTTON_THRESHOLD6 (2250)
#define BUTTON_THRESHOLD7 (2700)
#define BUTTON_THRESHOLD8 (3300)
#define ANALOG_READ_MAX   (4095)

/* Colors */
#define EMPTY_COLOR        (0)
#define PLAYER1_COLOR      (1)
#define PLAYER2_COLOR      (2)
#define PLAYER1_KING_COLOR (3)
#define PLAYER2_KING_COLOR (4)

/**********************************
 ** Global Variables
 **********************************/
/* LED Control variables */
LedControl red_lc = LedControl(LED_MAX_CHIP_RED_PIN, LED_MAX_CHIP_CLK_PIN, LED_MAX_CHIP_CS_PIN, 1);
LedControl blue_lc = LedControl(LED_MAX_CHIP_BLUE_PIN, LED_MAX_CHIP_CLK_PIN, LED_MAX_CHIP_CS_PIN, 1);
LedControl green_lc = LedControl(LED_MAX_CHIP_GREEN_PIN, LED_MAX_CHIP_CLK_PIN, LED_MAX_CHIP_CS_PIN, 1);

/**********************************
 ** Private Function Prototypes
 **********************************/
void IO_MapToMaxChip(int row, int col, int &max_row, int &max_col);

/**********************************
 ** Function Definitions
 **********************************/
/**
 * Convert the Checkerboard row/column to translate to the MAX row/column to set
 *
 * @param row: The row of the Checkerboard
 * @param col: The column of the Checkerboard
 * @param max_row: The row of the MAX chip to set to be returned
 * @param max_col: The column of the MAX chip to set to be returned
 */
void IO_MapToMaxChip(int row, int col, int &max_row, int &max_col) {
  /* MAX rows will be on their orginal column divided by 2 on the MAX chip */
  max_col = (int)(row / 2);

  /* Check if the row is even or odd to determine where the columns map to on the MAX chip */
  if (row % 2 == 0) {
    /* MAX rows will take up the first 4 rows on the MAX chip if their row is divisible by 2 */
    max_row = (int)(col / 2);
  }
  else {
    /* MAX rows will take up the last 4 rows on the MAX chip if their column is not divisible by 2 */
    max_row = (int)((col / 2) + 4);
  }
}

/**
 * Converts a move command string to a form of a 2d integer array
 *
 * @param move_string: The move command string being converted
 * @param move_int: The move command returned as a form of a 2d integer array
 */
void IO_ConvertMapToIndices(String (&move_string)[2], int (&move_int)[2][2]) {
  move_int[0][0] = move_string[0].charAt(0) - 65; /* A is 65 in the ASCII code, corresponds with row 0 */
  move_int[0][1] = move_string[0].charAt(1) - 49; /* 1 is 49 in the ASCII code, corresponds with column 0 */
  move_int[1][0] = move_string[1].charAt(0) - 65; /* A is 65 in the ASCII code, corresponds with row 0 */
  move_int[1][1] = move_string[1].charAt(1) - 49; /* 1 is 49 in the ASCII code, corresponds with column 0 */
}

/**
 * Checks for a voice command and make sure it follows the right format
 *
 * @param move_command: The move command being returned
 */
void IO_GetVoiceRecognitionInput(String (&move_command)[2]) {
  /* Get voice command from Voice Recognition module */
  String voice_command;
  VoiceRecognition_GetInput(voice_command);

  /* Voice commands should be in the format of A1 B2. If not they should be cleared */
  if ((voice_command.charAt(0) >= 'A' && voice_command.charAt(0) <= 'H') &&
      (voice_command.charAt(1) >= '1' && voice_command.charAt(1) <= '8') &&
      (voice_command.charAt(2) == ' ') &&
      (voice_command.charAt(3) >= 'A' && voice_command.charAt(3) <= 'H') &&
      (voice_command.charAt(4) >= '1' && voice_command.charAt(4) <= '8')) {
    /* Array should be cleared */
    move_command[0] = voice_command.substring(0, 2);
    move_command[1] = voice_command.substring(3, 5);
  }
  else {
    move_command[0] = "";
    move_command[1] = "";
  }
}

/**
 * Initializes the button pins
 *
 */
void IO_InitButton() {
  pinMode(BUTTON_ARRAY_PIN1, INPUT);
  pinMode(BUTTON_ARRAY_PIN2, INPUT);
  pinMode(BUTTON_ARRAY_PIN3, INPUT);
  pinMode(BUTTON_ARRAY_PIN4, INPUT);
  pinMode(BUTTON_POWER_PIN, OUTPUT);

  digitalWrite(BUTTON_POWER_PIN, HIGH);
}

/**
 * Checks if any buttons have been pressed
 *
 * @return String: The string command to return
 */
String IO_GetButtonInput() {
  String move_queue = "";
  bool button1_pressed = true;
  bool button2_pressed = true;
  bool button3_pressed = true;
  bool button4_pressed = true;
  
  /* Button array 1 for rows 0 and 1 */
  if (analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD1) {
    /* Button for [0, 0] */
    move_queue = "A1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [0, 2] */
    move_queue = "A3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [0, 4] */
    move_queue = "A5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [0, 6] */
    move_queue = "A7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [1, 1] */
    move_queue = "B2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [1, 3] */
    move_queue = "B4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [1, 5] */
    move_queue = "B6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [1, 7] */
    move_queue = "B8";
  }
  else {
    /* Button in the first row of buttons were not pressed */
    button1_pressed = false;
  }

  /* Button array 2 for rows 2 and 3 */
  if (analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD1) {
    /* Button for [2, 0] */
    move_queue = "C1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD2) {
    /* Button for [2, 2] */
    move_queue = "C3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD3) {
    /* Button for [2, 4] */
    move_queue = "C5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD4) {
    /* Button for [2, 6] */
    move_queue = "C7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD5) {
    /* Button for [3, 1] */
    move_queue = "D2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD6) {
    /* Button for [3, 3] */
    move_queue = "D4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD7) {
    /* Button for [3, 5] */
    move_queue = "D6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD8) {
    /* Button for [3, 7] */
    move_queue = "D8";
  }
  else {
    /* Button in the second row of buttons were not pressed */
    button2_pressed = false;
  }

  /* Button array 3 for rows 4 and 5 */
  if (analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD1) {
    /* Button for [4, 0] */
    move_queue = "E1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD2) {
    /* Button for [4, 2] */
    move_queue = "E3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD3) {
    /* Button for [4, 4] */
    move_queue = "E5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD4) {
    /* Button for [4, 6] */
    move_queue = "E7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD5) {
    /* Button for [5, 1] */
    move_queue = "F2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD6) {
    /* Button for [5, 3] */
    move_queue = "F4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD7) {
    /* Button for [5, 5] */
    move_queue = "F6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD8) {
    /* Button for [5, 7] */
    move_queue = "F8";
  }
  else {
    /* Button in the third row of buttons were not pressed */
    button3_pressed = false;
  }
  
  /* Button array 4 for rows 6 and 7 */
  if (analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD1) {
    /* Button for [6, 0] */
    move_queue = "G1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD2) {
    /* Button for [6, 2] */
    move_queue = "G3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD3) {
    /* Button for [6, 4] */
    move_queue = "G5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD4) {
    /* Button for [6, 6] */
    move_queue = "G7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD5) {
    /* Button for [7, 1] */
    move_queue = "H2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD6) {
    /* Button for [7, 3] */
    move_queue = "H4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD7) {
    /* Button for [7, 5] */
    move_queue = "H6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD8) {
    /* Button for [7, 7] */
    move_queue = "H8";
  }
  else {
    /* Button in the fourth row of buttons were not pressed */
    button4_pressed = false;
  }

  /* Verify only one output pin is getting one accepted analog reading at a time */
  if ((button1_pressed == true && button2_pressed == true) || (button1_pressed == true && button3_pressed == true) ||
      (button1_pressed == true && button4_pressed == true) || (button2_pressed == true && button3_pressed == true) ||
      (button2_pressed == true && button4_pressed == true) || (button3_pressed == true && button4_pressed == true)) {
    move_queue = "";
  }

  /* Delay to account for button press and debounce */
  delay(100);
  
  return move_queue;
}

/**
 * Initializes the turn indicator LED pins
 *
 */
void IO_InitTurnIndicator() {
  pinMode(PLAYER1_TURN_INDICATOR_LED_PIN, OUTPUT);
  pinMode(PLAYER2_TURN_INDICATOR_LED_PIN, OUTPUT);
}

/**
 * Will update the LED indicating the turn indicators throughout each process
 *
 * @param player_turn: The turn of the current player
 */
void IO_SetTurnIndicator(int player_turn) {
  /* Get player turn from game algorithm and update */
  if (player_turn == 1) {
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH); /* Set LED1 GPIO pin to high */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);  /* Set LED2 GPIO pin to low */
  }
  else if (player_turn == 2) {
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);  /* Set LED1 GPIO pin to low */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH); /* Set LED2 GPIO pin to high */
  }
}

/**
 * Will blink the turn indicator if an invalid move is asked for
 *
 * @param player_turn: The turn of the current player
 */
void IO_BlinkTurnIndicator(int player_turn) {
  /* Get player turn from game algorithm and update */
  if (player_turn == 1) {
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);
    delay(200);
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH);
    delay(200);
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);
    delay(200);
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH);
  }
  else if (player_turn == 2) {
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);
    delay(200);
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH);
    delay(200);
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);
    delay(200);
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH);
  }
}

/**
 * Will alternate the turn indicator LED for the winner
 *
 * @param winner: The player who won the game
 */
void IO_WinnerTurnIndicator(int winner) {
  if (winner == 1) {
    /* Set LED2 GPIO pin to low */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);
    /* Alternate LED1 GPIO pin to indicate player 1 is winner */
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);
    delay(1000);
  }
  else {
    /* Set LED1 GPIO pin to low */
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);
    /* Alternate LED2 GPIO pin to indicate player 2 is winner */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);
    delay(1000);
  }
}

/**
 * Initializes the game map LEDs
 *
 */
void IO_InitHWGameMap() {
  /* Initialize the red LED max chip (via LED control) */
  red_lc.shutdown(0, false);
  red_lc.setIntensity(0, 15);
  red_lc.clearDisplay(0);

  /* Initialize the blue LED max chip (via LED control) */
  blue_lc.shutdown(0, false);
  blue_lc.setIntensity(0, 15);
  blue_lc.clearDisplay(0);

  /* Initialize the green LED max chip (via LED control) */
  green_lc.shutdown(0, false);
  green_lc.setIntensity(0, 15);
  green_lc.clearDisplay(0);
}

/**
 * Update the RGB LEDs corresponding to the game map
 *
 * @param checker_game: The checker game that the board is being retrieved from
 */
void IO_SetHWGameMap(Checkers checker_game) {
  int max_row = -1;
  int max_col = -1;

  /* Get game map from game algorithm and update */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row + col) % 2 == 0) {
        /* Get MAX chip row and column */
        IO_MapToMaxChip(row, col, max_row, max_col);

        switch (checker_game.Checkers_GetBoardAt(row, col)) {
          case EMPTY_COLOR: /* No piece */
            red_lc.setLed(0, max_row, max_col, false);
            green_lc.setLed(0, max_row, max_col, false);
            blue_lc.setLed(0, max_row, max_col, false);
            break;
          case PLAYER1_COLOR: /* Player 1 regular piece */
            /* Red */
            red_lc.setLed(0, max_row, max_col, true);
            green_lc.setLed(0, max_row, max_col, false);
            blue_lc.setLed(0, max_row, max_col, false);
            break;
          case PLAYER2_COLOR: /* Player 2 regular piece */
            /* Blue */
            red_lc.setLed(0, max_row, max_col, false);
            green_lc.setLed(0, max_row, max_col, false);
            blue_lc.setLed(0, max_row, max_col, true);
            break;
          case PLAYER1_KING_COLOR: /* Player 1 king piece */
            /* Yellow */
            red_lc.setLed(0, max_row, max_col, true);
            green_lc.setLed(0, max_row, max_col, true);
            blue_lc.setLed(0, max_row, max_col, false);
            break;
          case PLAYER2_KING_COLOR: /* Player 2 king piece */
            /* Light Blue */
            red_lc.setLed(0, max_row, max_col, false);
            green_lc.setLed(0, max_row, max_col, true);
            blue_lc.setLed(0, max_row, max_col, true);
            break;
          default:
            break;
        }
      }
    }
  }
}
