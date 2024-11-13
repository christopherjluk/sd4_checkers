/************************************************************
 * @file Test_Checkers.ino
 * @brief The tests for the game algorithm module
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "ArduinoUnit.h"

/**********************************
 ** Tests
 **********************************/
/**
 * Checkers constructor tests
 **/
test(Checkers_Constructor_Success) {
  Checkers checkers_game;

  /* Check initial values */
  assertEqual(checkers_game.Checkers_GetP1Count(), 12);
  assertEqual(checkers_game.Checkers_GetP2Count(), 12);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 1);
  assertEqual(checkers_game.jump_lock[2], 0);
  assertEqual(checkers_game.Checkers_GetWinner(), 0);
  
  /* Check the game board */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((i > 4) && ((i + j) % 2 == 0)) {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 1);
      }
      else if ((i < 3) && ((i + j) % 2 == 0)) {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 2);
      }
      else {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 0);
      }
    }
  }
}

/**********************************
 ** Function Definitions
 **********************************/
/**
 * Set up serial to receive test results
 *
 * @note Must be named "setup" so the MCU knows to run this first before running the loop
 */
void setup() {
  Serial.begin(115200);
  while(!Serial) {}
}

/**
 * Will loop through and run the tests, printing the results
 *
 * @note Must be named "loop" so it will repeatedly run on the MCU
 */
void loop() {
  Test::run();
}
