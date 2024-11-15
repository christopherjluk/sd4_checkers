/************************************************************
 * @file Test_Io.ino
 * @brief The tests for the I/O module
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Io.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "ArduinoUnit.h"

/**********************************
 ** Tests
 **********************************/
/**
 * IO_MapToMaxChip tests
 **/
test(IO_MapToMaxChip_Success) {
  int expected_max_row = -1;
  int expected_max_col = -1;
  int max_row = -1;
  int max_col = -1;

  /* Tests even rows */
  for (int row = 0; row < 8; row = row + 2) {
    for (int col = 0; col < 8; col = col + 2) {
      IO_MapToMaxChip(row, col, max_row, max_col);

      expected_max_col = (int)(row / 2);
      expected_max_row = (int)(col / 2);

      assertEqual(expected_max_row, max_row);
      assertEqual(expected_max_col, max_col);
    }
  }

  /* Tests odd rows */
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

/**
 * IO_ConvertMapToIndices tests
 **/
test(IO_ConvertMapToIndices_Success) {
  String move_string_1[2] = {"A1", "B2"};
  String move_string_2[2] = {"C8", "C7"};
  String move_string_3[2] = {"D4", "H1"};
  String move_string_4[2] = {"E5", "G3"};
  String move_string_5[2] = {"B6", "F3"};

  int move_int_1[2][2];
  int move_int_2[2][2];
  int move_int_3[2][2];
  int move_int_4[2][2];
  int move_int_5[2][2];

  IO_ConvertMapToIndices(move_string_1, move_int_1);
  IO_ConvertMapToIndices(move_string_2, move_int_2);
  IO_ConvertMapToIndices(move_string_3, move_int_3);
  IO_ConvertMapToIndices(move_string_4, move_int_4);
  IO_ConvertMapToIndices(move_string_5, move_int_5);

  /* Verify integer array is equivalent to expected values */
  assertEqual(move_int_1[0][0], 0);
  assertEqual(move_int_1[0][1], 0);
  assertEqual(move_int_1[1][0], 1);
  assertEqual(move_int_1[1][1], 1);
  assertEqual(move_int_2[0][0], 2);
  assertEqual(move_int_2[0][1], 7);
  assertEqual(move_int_2[1][0], 2);
  assertEqual(move_int_2[1][1], 6);
  assertEqual(move_int_3[0][0], 3);
  assertEqual(move_int_3[0][1], 3);
  assertEqual(move_int_3[1][0], 7);
  assertEqual(move_int_3[1][1], 0);
  assertEqual(move_int_4[0][0], 4);
  assertEqual(move_int_4[0][1], 4);
  assertEqual(move_int_4[1][0], 6);
  assertEqual(move_int_4[1][1], 2);
  assertEqual(move_int_5[0][0], 1);
  assertEqual(move_int_5[0][1], 5);
  assertEqual(move_int_5[1][0], 5);
  assertEqual(move_int_5[1][1], 2);
}

/**
 * IO_GetVoiceRecognitionInput tests
 **/
test(IO_GetVoiceRecognitionInput_Success) {
  String test_input_1 = "A1 B2";
  String test_input_2 = "E";
  String test_input_3 = "C6 E4";
  String test_input_4 = "";

  String move_command_1[2];
  String move_command_2[2];
  String move_command_3[2];
  String move_command_4[2];

  IO_GetVoiceRecognitionInput(test_input_1, move_command_1);
  IO_GetVoiceRecognitionInput(test_input_2, move_command_2);
  IO_GetVoiceRecognitionInput(test_input_3, move_command_3);
  IO_GetVoiceRecognitionInput(test_input_4, move_command_4);

  /* Verify outputted move command is equivalent to expected values */
  assertEqual(move_command_1[0], "A1");
  assertEqual(move_command_1[1], "B2");
  assertEqual(move_command_2[0], "");
  assertEqual(move_command_2[1], "");
  assertEqual(move_command_3[0], "C6");
  assertEqual(move_command_3[1], "E4");
  assertEqual(move_command_4[0], "");
  assertEqual(move_command_4[1], "");
}

/**
 * IO_InitButton tests
 **/
test(IO_InitButton_Success) {
  int pin_adder = 0;
  int input_counter = 0;
  int output_counter = 0;
  int low_counter = 0;
  int high_counter = 0;

  IO_InitButton(pin_adder, input_counter, output_counter, low_counter, high_counter);

  /* Verify the pins called and the inputs and outputs set are correct, along with the low and high */
  assertEqual(pin_adder, 208);
  assertEqual(input_counter, 4);
  assertEqual(output_counter, 1);
  assertEqual(low_counter, 0);
  assertEqual(high_counter, 1);
}

/**
 * IO_GetButtonInput tests
 **/

/**
 * IO_InitTurnIndicator tests
 **/
test(IO_InitTurnIndicator_Success) {
  int pin_adder = 0;
  int input_counter = 0;
  int output_counter = 0;

  IO_InitTurnIndicator(pin_adder, input_counter, output_counter);

  /* Verify the pins called and the inputs and outputs set are correct */
  assertEqual(pin_adder, 25);
  assertEqual(input_counter, 0);
  assertEqual(output_counter, 2);
}

/**
 * IO_SetTurnIndicator tests
 **/
test(IO_SetTurnIndicator_Success) {
  /* For player 1 */
  int pin_adder = 0;
  int low_counter = 0;
  int high_counter = 0;

  IO_SetTurnIndicator(1, pin_adder, low_counter, high_counter);

  /* Verify the pins called and the lows and highs set are correct */
  assertEqual(pin_adder, 25);
  assertEqual(low_counter, 1);
  assertEqual(high_counter, 1);

  /* For player 2 */
  pin_adder = 0;
  low_counter = 0;
  high_counter = 0;

  IO_SetTurnIndicator(2, pin_adder, low_counter, high_counter);

  /* Verify the pins called and the lows and highs set are correct */
  assertEqual(pin_adder, 25);
  assertEqual(low_counter, 1);
  assertEqual(high_counter, 1);
}

/**
 * IO_BlinkTurnIndicator tests
 **/
test(IO_BlinkTurnIndicator_Success) {
  /* For player 1 */
  int pin_adder = 0;
  int low_counter = 0;
  int high_counter = 0;
  int delay_adder = 0;

  IO_BlinkTurnIndicator(1, pin_adder, low_counter, high_counter, delay_adder);

  /* Verify the pins called and the lows and highs set are correct with the delays set to the correct times */
  assertEqual(pin_adder, 48);
  assertEqual(low_counter, 2);
  assertEqual(high_counter, 2);
  assertEqual(delay_adder, 600);

  /* For player 2 */
  pin_adder = 0;
  low_counter = 0;
  high_counter = 0;
  delay_adder = 0;

  IO_BlinkTurnIndicator(2, pin_adder, low_counter, high_counter, delay_adder);

  /* Verify the pins called and the lows and highs set are correct with the delays set to the correct times */
  assertEqual(pin_adder, 52);
  assertEqual(low_counter, 2);
  assertEqual(high_counter, 2);
  assertEqual(delay_adder, 600);
}

/**
 * IO_WinnerTurnIndicator tests
 **/
test(IO_WinnerTurnIndicator_Success) {
  /* For player 1 */
  int pin_adder = 0;
  int low_counter = 0;
  int high_counter = 0;
  int delay_adder = 0;

  IO_WinnerTurnIndicator(1, pin_adder, low_counter, high_counter, delay_adder);

  /* Verify the pins called and the lows and highs set are correct with the delays set to the correct times */
  assertEqual(pin_adder, 37);
  assertEqual(low_counter, 2);
  assertEqual(high_counter, 1);
  assertEqual(delay_adder, 2000);

  /* For player 2 */
  pin_adder = 0;
  low_counter = 0;
  high_counter = 0;
  delay_adder = 0;

  IO_WinnerTurnIndicator(2, pin_adder, low_counter, high_counter, delay_adder);

  /* Verify the pins called and the lows and highs set are correct with the delays set to the correct times */
  assertEqual(pin_adder, 38);
  assertEqual(low_counter, 2);
  assertEqual(high_counter, 1);
  assertEqual(delay_adder, 2000);
}

/**
 * IO_InitHWGameMap tests
 **/
test(IO_InitHWGameMap_Success) {
  bool function_called_correctly = false;
  int red_counter = 0;
  int blue_counter = 0;
  int green_counter = 0;
  
  IO_InitHWGameMap(function_called_correctly, red_counter, blue_counter, green_counter);

  assertEqual(function_called_correctly, true);
  assertEqual(red_counter, 3);
  assertEqual(blue_counter, 3);
  assertEqual(green_counter, 3);
}

/**
 * IO_SetHWGameMap tests
 **/
test(IO_SetHWGameMap_Success) {
  int board[8][8];
  
  for (int i = 0; i < 8; i++) {   /* For iterating through the rows */
    for (int j = 0; j < 8; j++) { /* For iterating through the columns */
      /* Initializes player 1's pieces */
      if (i > 4 && ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))) {
        board[i][j] = 1;
      }
      /* Initializes player 2's pieces */
      else if (i < 3 && ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))) {
        board[i][j] = 2;
      }
      /* Initializes empty squares */
      else {
        board[i][j] = 0;
      }
    }
  }

  bool function_called_correctly = false;
  int red_counter = 0;
  int blue_counter = 0;
  int green_counter = 0;
  int true_counter = 0;
  int row_adder = 0;
  int col_adder = 0;

  IO_SetHWGameMap(board, function_called_correctly, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);

  assertEqual(function_called_correctly, true);
  assertEqual(red_counter, 64);
  assertEqual(blue_counter, 64);
  assertEqual(green_counter, 64);
  assertEqual(true_counter, 24);
  assertEqual(row_adder, 336);
  assertEqual(col_adder, 144);
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
