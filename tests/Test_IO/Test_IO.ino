/************************************************************
 * @file Test_IO.ino
 * @brief The tests for the IO module
 ************************************************************/

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include <ArduinoUnit.h>

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

/**********************************
 ** Tests
 **********************************/
/**
 * IO_MapToMaxChip tests
 **/
test(IO_MapToMaxChip_EvenRows) {
  int expected_max_row = -1;
  int expected_max_col = -1;
  int max_row = -1;
  int max_col = -1;

  for (int row = 0; row < 8; row = row + 2) {
    for (int col = 0; col < 8; col = col + 2) {
      IO_MapToMaxChip(row, col, max_row, max_col);

      expected_max_col = (int)(row / 2);
      expected_max_row = (int)(col / 2);

      assertEqual(expected_max_row, max_row);
      assertEqual(expected_max_col, max_col);
    }
  }
}

test(IO_MapToMaxChip_OddRows) {
  int expected_max_row = -1;
  int expected_max_col = -1;
  int max_row = -1;
  int max_col = -1;

  for (int row = 1; row < 8; row = row + 2) {
    for (int col = 0; col < 8; col = col + 2) {
      IO_MapToMaxChip(row, col, max_row, max_col);

      expected_max_col = (int)(row / 2);
      expected_max_row = (int)((col / 2) + 4);

      assertEqual(expected_max_row, max_row);
      assertEqual(expected_max_col, max_col);
    }
  }
}

/**********************************
 ** Looping Process
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
