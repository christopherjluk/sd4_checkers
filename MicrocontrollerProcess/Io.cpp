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
#define PLAYER1_TURN_INDICATOR_LED_PIN (12)
#define PLAYER2_TURN_INDICATOR_LED_PIN (13)
#define LED_MAX_CHIP_RED_PIN           (33)
#define LED_MAX_CHIP_GREEN_PIN         (25)
#define LED_MAX_CHIP_BLUE_PIN          (26)
#define LED_MAX_CHIP_CS_PIN            (27)
#define LED_MAX_CHIP_CLK_PIN           (14)

/* Button array thresholds (Subject to change) */
#define BUTTON_THRESHOLD1 (5)
#define BUTTON_THRESHOLD2 (10)
#define BUTTON_THRESHOLD3 (15)
#define BUTTON_THRESHOLD4 (20)
#define BUTTON_THRESHOLD5 (25)
#define BUTTON_THRESHOLD6 (30)
#define BUTTON_THRESHOLD7 (35)
#define BUTTON_THRESHOLD8 (40)
#define ANALOG_READ_MAX   (4096)

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
 ** Function Definitions
 **********************************/
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
  VoiceRecognition_GetInput(move_command);
  
  /* Voice commands should be in the format of A1 B2. If not they should be cleared */
  if (!(move_command[0].charAt(0) >= 'A' && move_command[0].charAt(0) <= 'F') ||
      !(move_command[0].charAt(1) >= '1' && move_command[0].charAt(1) <= '8') ||
      !(move_command[1].charAt(0) >= 'A' && move_command[1].charAt(0) <= 'F') ||
      !(move_command[1].charAt(1) >= '1' && move_command[1].charAt(1) <= '8')) {
    /* Array should be cleared */
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
}

/**
 * Checks if any buttons have been pressed
 *
 * @return String: The string command to return
 */
String IO_GetButtonInput() {
  String move_queue = "";
  
  /* Button array 1 for rows 1 and 2 */
  if (analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD1) {
    /* Button for [1, 1] */
    move_queue = "A1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [1, 3] */
    move_queue = "A3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [1, 5] */
    move_queue = "A5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [1, 7] */
    move_queue = "A7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [2, 2] */
    move_queue = "B2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [2, 4] */
    move_queue = "B4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [2, 6] */
    move_queue = "B6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [2, 8] */
    move_queue = "B8";
  }

  /* Button array 2 for rows 3 and 4 */
  if (analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD1) {
    /* Button for [3, 1] */
    move_queue = "C1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [3, 3] */
    move_queue = "C3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [3, 5] */
    move_queue = "C5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [3, 7] */
    move_queue = "C7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [4, 2] */
    move_queue = "D2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [4, 4] */
    move_queue = "D4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [4, 6] */
    move_queue = "D6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [4, 8] */
    move_queue = "D8";
  }

  /* Button array 3 for rows 5 and 6 */
  if (analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD1) {
    /* Button for [5, 1] */
    move_queue = "E1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [5, 3] */
    move_queue = "E3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [5, 5] */
    move_queue = "E5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [5, 7] */
    move_queue = "E7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [6, 2] */
    move_queue = "F2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [6, 4] */
    move_queue = "F4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [6, 6] */
    move_queue = "F6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [6, 8] */
    move_queue = "F8";
  }
  
  /* Button array 4 for rows 7 and 8 */
  if (analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD1) {
    /* Button for [7, 1] */
    move_queue = "G1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [7, 3] */
    move_queue = "G3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [7, 5] */
    move_queue = "G5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [7, 7] */
    move_queue = "G7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [8, 2] */
    move_queue = "H2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [8, 4] */
    move_queue = "H4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [8, 6] */
    move_queue = "H6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [8, 8] */
    move_queue = "H8";
  }

  /* Verify only one output pin is getting one accepted analog reading at a time */
  if (analogRead(BUTTON_ARRAY_PIN1) < ANALOG_READ_MAX && analogRead(BUTTON_ARRAY_PIN2) < ANALOG_READ_MAX &&
      analogRead(BUTTON_ARRAY_PIN3) < ANALOG_READ_MAX && analogRead(BUTTON_ARRAY_PIN4) < ANALOG_READ_MAX) {
    move_queue = "";
  }
  
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
 * Update the RGB LEDs corresponding to the game map
 *
 * @param checker_game: The checker game that the board is being retrieved from
 * @note This may need to be updated based on the PCB design
 */
void IO_SetHWGameMap(Checkers checker_game) {
  /* Get game map from game algorithm and update */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      switch (checker_game.Checkers_GetBoardAt(row, col)) {
        case EMPTY_COLOR: /* No piece */
          red_lc.setLed(0, row, col, false);
          green_lc.setLed(0, row, col, false);
          blue_lc.setLed(0, row, col, false);
          break;
        case PLAYER1_COLOR: /* Player 1 regular piece */
          /* Red */
          red_lc.setLed(0, row, col, true);
          green_lc.setLed(0, row, col, false);
          blue_lc.setLed(0, row, col, false);
          break;
        case PLAYER2_COLOR: /* Player 2 regular piece */
          /* Blue */
          red_lc.setLed(0, row, col, false);
          green_lc.setLed(0, row, col, false);
          blue_lc.setLed(0, row, col, true);
          break;
        case PLAYER1_KING_COLOR: /* Player 1 king piece */
          /* Yellow */
          red_lc.setLed(0, row, col, true);
          green_lc.setLed(0, row, col, true);
          blue_lc.setLed(0, row, col, false);
          break;
        case PLAYER2_KING_COLOR: /* Player 2 king piece */
          /* Light Blue */
          red_lc.setLed(0, row, col, false);
          green_lc.setLed(0, row, col, true);
          blue_lc.setLed(0, row, col, true);
          break;
        default:
          break;
      }
    }
  }
}
