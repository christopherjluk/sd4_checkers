/************************************************************
 * @file Io.cpp
 * @brief The implementation for I/O related functionalities
 * @note This file is copied over from src and modified for testing
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Io.h";

/**********************************
 ** Third Party Libraries Includes
 **********************************/

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

/* Arduino mocks */
#define INPUT_MOCK  (1)
#define OUTPUT_MOCK (2)
#define LOW_MOCK    (1)
#define HIGH_MOCK   (2)

/* LED control mocks */
#define RED_MOCK   (1)
#define BLUE_MOCK  (2)
#define GREEN_MOCK (3)

/* Colors */
#define EMPTY_COLOR        (0)
#define PLAYER1_COLOR      (1)
#define PLAYER2_COLOR      (2)
#define PLAYER1_KING_COLOR (3)
#define PLAYER2_KING_COLOR (4)

/**********************************
 ** Global Variables
 **********************************/

/**********************************
 ** Helper Functions
 **********************************/
/**
 * This function will mock a pinMode call
 *
 * @param pin: The "pin" to configure
 * @param mode: The "mode" to set the pin to
 * @param pin_adder: The sum of the pin numbers being set
 * @param input_counter: The number of input pins
 * @param output_counter: The number of output pins
 */
void pinModeMock(int pin, int mode, int &pin_adder, int &input_counter, int &output_counter) {
  pin_adder += pin;
  
  if (mode == INPUT_MOCK) {
    input_counter++;
  }
  else if (mode == OUTPUT_MOCK) {
    output_counter++;
  }
}

/**
 * This function will mock a digitalWrite call
 *
 * @param pin: The "pin" to write to
 * @param output: The "output" to set the pin to
 * @param pin_adder: The sum of the pin numbers being written to
 * @param low_counter: The number of input pins
 * @param high_counter: The number of output pins
 */
void digitalWriteMock(int pin, int output, int &pin_adder, int &low_counter, int &high_counter) {
  pin_adder += pin;

  if (output == LOW_MOCK) {
    low_counter++;
  }
  else {
    high_counter++;
  }
}

/**
 * This function will mock a delay call
 *
 * @param time: The delay time
 * @param delay_adder: The sum of the delay time
 */
void delayMock(int time, int &delay_adder) {
  delay_adder += time;
}

/**
 * This function will mock an analog read call
 *
 * @param pin: The "pin" to write to
 * @param reading: The mocked reading
 * @param pin_adder: The sum of the pin numbers being written to
 * @param reading_adder: The sum of the readings
 * @return int: The returned reading
 */
int analogReadMock(int pin, int reading, int &pin_adder, int &reading_adder) {
  pin_adder += pin;
  reading_adder += reading;

  return reading;
}

/**
 * This function will mock a lc shutdown call
 *
 * @param color: The mocked color being called
 * @param device_num: The number of the device in the LC
 * @param shutdown: Whether it is shutdown mode or not
 * @param red_counter: How many times the red LC was called
 * @param blue_counter: How many times the blue LC was called
 * @param green_counter: How many times the green LC was called
 * @return bool: Whether the function was called correctly
 */
bool lcShutdownMock(int color, int device_num, bool shutdown, int &red_counter, int &blue_counter, int &green_counter) {
  if (color == RED_MOCK) {
    red_counter++;
  }
  else if (color == BLUE_MOCK) {
    blue_counter++;
  }
  else if (color == GREEN_MOCK) {
    green_counter++;
  }

  if (device_num == 0 && shutdown == false) {
    return true;
  }

  return false;
}

/**
 * This function will mock a lc set intensity call
 *
 * @param color: The mocked color being called
 * @param device_num: The number of the device in the LC
 * @param intensity: The intensity of the LED
 * @param red_counter: How many times the red LC was called
 * @param blue_counter: How many times the blue LC was called
 * @param green_counter: How many times the green LC was called
 * @return bool: Whether the function was called correctly
 */
bool lcSetIntensityMock(int color, int device_num, int intensity, int &red_counter, int &blue_counter, int &green_counter) {
  if (color == RED_MOCK) {
    red_counter++;
  }
  else if (color == BLUE_MOCK) {
    blue_counter++;
  }
  else if (color == GREEN_MOCK) {
    green_counter++;
  }

  if (device_num == 0 && intensity == 15) {
    return true;
  }

  return false;
}

/**
 * This function will mock a lc clear display call
 *
 * @param color: The mocked color being called
 * @param device_num: The number of the device in the LC
 * @param red_counter: How many times the red LC was called
 * @param blue_counter: How many times the blue LC was called
 * @param green_counter: How many times the green LC was called
 * @return bool: Whether the function was called correctly
 */
bool lcClearDisplayMock(int color, int device_num, int &red_counter, int &blue_counter, int &green_counter) {
  if (color == RED_MOCK) {
    red_counter++;
  }
  else if (color == BLUE_MOCK) {
    blue_counter++;
  }
  else if (color == GREEN_MOCK) {
    green_counter++;
  }

  if (device_num == 0) {
    return true;
  }

  return false;
}

/**
 * This function will mock a lc set led call
 *
 * @param color: The mocked color being called
 * @param device_num: The number of the device in the LC
 * @param row: The MAX row to set
 * @param col: The MAX col to set
 * @param status: The status to set the LEDs
 * @param red_counter: How many times the red LC was called
 * @param blue_counter: How many times the blue LC was called
 * @param green_counter: How many times the green LC was called
 * @param true_counter: How many trues were called
 * @param row_counter: The rows added up
 * @param col_counter: The cols added up
 * @return bool: Whether the function was called correctly
 */
bool lcSetLedMock(int color, int device_num, int row, int col, bool status, int &red_counter, int &blue_counter, int &green_counter, int &true_counter, int &row_adder, int &col_adder) {
  if (color == RED_MOCK) {
    red_counter++;
  }
  else if (color == BLUE_MOCK) {
    blue_counter++;
  }
  else if (color == GREEN_MOCK) {
    green_counter++;
  }

  if (status == true) {
    true_counter++;
  }

  row_adder += row;
  col_adder += col;

  if (device_num == 0) {
    return true;
  }

  return false;
}

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
 * @param test_input: The string being stubbed in for the voice recognition input
 * @param move_command: The move command being returned
 */
void IO_GetVoiceRecognitionInput(String test_input, String (&move_command)[2]) {
  /* Get voice command from Voice Recognition module */
  String voice_command = test_input;

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
 * @param pin_adder: The sum of the pin numbers being set
 * @param input_counter: The number of input pins
 * @param output_counter: The number of output pins
 */
void IO_InitButton(int &pin_adder, int &input_counter, int &output_counter, int &low_counter, int &high_counter) {
  pinModeMock(BUTTON_ARRAY_PIN1, INPUT_MOCK, pin_adder, input_counter, output_counter);
  pinModeMock(BUTTON_ARRAY_PIN2, INPUT_MOCK, pin_adder, input_counter, output_counter);
  pinModeMock(BUTTON_ARRAY_PIN3, INPUT_MOCK, pin_adder, input_counter, output_counter);
  pinModeMock(BUTTON_ARRAY_PIN4, INPUT_MOCK, pin_adder, input_counter, output_counter);
  pinModeMock(BUTTON_POWER_PIN, OUTPUT_MOCK, pin_adder, input_counter, output_counter);

  digitalWriteMock(BUTTON_POWER_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
}

/**
 * Checks if any buttons have been pressed
 *
 * @param reading: The readings to mock
 * @param pin_adder: The sum of the pins being read
 * @param reading_adder: The sum of the readings being read
 * @param delay_adder: The sum of the delay time
 * @return String: The string command to return
 */
String IO_GetButtonInput(int (&reading)[4], int &pin_adder, int &reading_adder, int &delay_adder) {
  String move_queue = "";
  bool button1_pressed = true;
  bool button2_pressed = true;
  bool button3_pressed = true;
  bool button4_pressed = true;

  int reading1 = analogReadMock(BUTTON_ARRAY_PIN1, reading[0], pin_adder, reading_adder);
  int reading2 = analogReadMock(BUTTON_ARRAY_PIN2, reading[1], pin_adder, reading_adder);
  int reading3 = analogReadMock(BUTTON_ARRAY_PIN3, reading[2], pin_adder, reading_adder);
  int reading4 = analogReadMock(BUTTON_ARRAY_PIN4, reading[3], pin_adder, reading_adder);
  
  /* Button array 1 for rows 0 and 1 */
  if (reading1 < BUTTON_THRESHOLD1) {
    /* Button for [0, 0] */
    move_queue = "A1";
  }
  else if (reading1 > BUTTON_THRESHOLD1 && reading1 < BUTTON_THRESHOLD2) {
    /* Button for [0, 2] */
    move_queue = "A3";
  }
  else if (reading1 > BUTTON_THRESHOLD2 && reading1 < BUTTON_THRESHOLD3) {
    /* Button for [0, 4] */
    move_queue = "A5";
  }
  else if (reading1 > BUTTON_THRESHOLD3 && reading1 < BUTTON_THRESHOLD4) {
    /* Button for [0, 6] */
    move_queue = "A7";
  }
  else if (reading1 > BUTTON_THRESHOLD4 && reading1 < BUTTON_THRESHOLD5) {
    /* Button for [1, 1] */
    move_queue = "B2";
  }
  else if (reading1 > BUTTON_THRESHOLD5 && reading1 < BUTTON_THRESHOLD6) {
    /* Button for [1, 3] */
    move_queue = "B4";
  }
  else if (reading1 > BUTTON_THRESHOLD6 && reading1 < BUTTON_THRESHOLD7) {
    /* Button for [1, 5] */
    move_queue = "B6";
  }
  else if (reading1 > BUTTON_THRESHOLD7 && reading1 < BUTTON_THRESHOLD8) {
    /* Button for [1, 7] */
    move_queue = "B8";
  }
  else {
    /* Button in the first row of buttons were not pressed */
    button1_pressed = false;
  }

  /* Button array 2 for rows 2 and 3 */
  if (reading2 < BUTTON_THRESHOLD1) {
    /* Button for [2, 0] */
    move_queue = "C1";
  }
  else if (reading2 > BUTTON_THRESHOLD1 && reading2 < BUTTON_THRESHOLD2) {
    /* Button for [2, 2] */
    move_queue = "C3";
  }
  else if (reading2 > BUTTON_THRESHOLD2 && reading2 < BUTTON_THRESHOLD3) {
    /* Button for [2, 4] */
    move_queue = "C5";
  }
  else if (reading2 > BUTTON_THRESHOLD3 && reading2 < BUTTON_THRESHOLD4) {
    /* Button for [2, 6] */
    move_queue = "C7";
  }
  else if (reading2 > BUTTON_THRESHOLD4 && reading2 < BUTTON_THRESHOLD5) {
    /* Button for [3, 1] */
    move_queue = "D2";
  }
  else if (reading2 > BUTTON_THRESHOLD5 && reading2 < BUTTON_THRESHOLD6) {
    /* Button for [3, 3] */
    move_queue = "D4";
  }
  else if (reading2 > BUTTON_THRESHOLD6 && reading2 < BUTTON_THRESHOLD7) {
    /* Button for [3, 5] */
    move_queue = "D6";
  }
  else if (reading2 > BUTTON_THRESHOLD7 && reading2 < BUTTON_THRESHOLD8) {
    /* Button for [3, 7] */
    move_queue = "D8";
  }
  else {
    /* Button in the second row of buttons were not pressed */
    button2_pressed = false;
  }

  /* Button array 3 for rows 4 and 5 */
  if (reading3 < BUTTON_THRESHOLD1) {
    /* Button for [4, 0] */
    move_queue = "E1";
  }
  else if (reading3 > BUTTON_THRESHOLD1 && reading3 < BUTTON_THRESHOLD2) {
    /* Button for [4, 2] */
    move_queue = "E3";
  }
  else if (reading3 > BUTTON_THRESHOLD2 && reading3 < BUTTON_THRESHOLD3) {
    /* Button for [4, 4] */
    move_queue = "E5";
  }
  else if (reading3 > BUTTON_THRESHOLD3 && reading3 < BUTTON_THRESHOLD4) {
    /* Button for [4, 6] */
    move_queue = "E7";
  }
  else if (reading3 > BUTTON_THRESHOLD4 && reading3 < BUTTON_THRESHOLD5) {
    /* Button for [5, 1] */
    move_queue = "F2";
  }
  else if (reading3 > BUTTON_THRESHOLD5 && reading3 < BUTTON_THRESHOLD6) {
    /* Button for [5, 3] */
    move_queue = "F4";
  }
  else if (reading3 > BUTTON_THRESHOLD6 && reading3 < BUTTON_THRESHOLD7) {
    /* Button for [5, 5] */
    move_queue = "F6";
  }
  else if (reading3 > BUTTON_THRESHOLD7 && reading3 < BUTTON_THRESHOLD8) {
    /* Button for [5, 7] */
    move_queue = "F8";
  }
  else {
    /* Button in the third row of buttons were not pressed */
    button3_pressed = false;
  }
  
  /* Button array 4 for rows 6 and 7 */
  if (reading3 < BUTTON_THRESHOLD1) {
    /* Button for [6, 0] */
    move_queue = "G1";
  }
  else if (reading3 > BUTTON_THRESHOLD1 && reading3 < BUTTON_THRESHOLD2) {
    /* Button for [6, 2] */
    move_queue = "G3";
  }
  else if (reading3 > BUTTON_THRESHOLD2 && reading3 < BUTTON_THRESHOLD3) {
    /* Button for [6, 4] */
    move_queue = "G5";
  }
  else if (reading3 > BUTTON_THRESHOLD3 && reading3 < BUTTON_THRESHOLD4) {
    /* Button for [6, 6] */
    move_queue = "G7";
  }
  else if (reading3 > BUTTON_THRESHOLD4 && reading3 < BUTTON_THRESHOLD5) {
    /* Button for [7, 1] */
    move_queue = "H2";
  }
  else if (reading3 > BUTTON_THRESHOLD5 && reading3 < BUTTON_THRESHOLD6) {
    /* Button for [7, 3] */
    move_queue = "H4";
  }
  else if (reading3 > BUTTON_THRESHOLD6 && reading3 < BUTTON_THRESHOLD7) {
    /* Button for [7, 5] */
    move_queue = "H6";
  }
  else if (reading3 > BUTTON_THRESHOLD7 && reading3 < BUTTON_THRESHOLD8) {
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
  delayMock(100, delay_adder);
  
  return move_queue;
}

/**
 * Initializes the turn indicator LED pins
 *
 * @param pin_adder: The sum of the pin numbers being set
 * @param input_counter: The number of input pins
 * @param output_counter: The number of output pins
 */
void IO_InitTurnIndicator(int &pin_adder, int &input_counter, int &output_counter) {
  pinModeMock(PLAYER1_TURN_INDICATOR_LED_PIN, OUTPUT_MOCK, pin_adder, input_counter, output_counter);
  pinModeMock(PLAYER2_TURN_INDICATOR_LED_PIN, OUTPUT_MOCK, pin_adder, input_counter, output_counter);
}

/**
 * Will update the LED indicating the turn indicators throughout each process
 *
 * @param player_turn: The turn of the current player
 * @param pin_adder: The sum of the pin numbers being set
 * @param low_counter: The number of low sets
 * @param high_counter: The number of high sets
 */
void IO_SetTurnIndicator(int player_turn, int &pin_adder, int &low_counter, int &high_counter) {
  /* Get player turn from game algorithm and update */
  if (player_turn == 1) {
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter); /* Set LED1 GPIO pin to high */
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);  /* Set LED2 GPIO pin to low */
  }
  else if (player_turn == 2) {
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);  /* Set LED1 GPIO pin to low */
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter); /* Set LED2 GPIO pin to high */
  }
}

/**
 * Will blink the turn indicator if an invalid move is asked for
 *
 * @param player_turn: The turn of the current player
 * @param pin_adder: The sum of the pin numbers being set
 * @param low_counter: The number of low sets
 * @param high_counter: The number of high sets
 * @param delay_adder: The sum of the delay time
 */
void IO_BlinkTurnIndicator(int player_turn, int &pin_adder, int &low_counter, int &high_counter, int &delay_adder) {
  /* Get player turn from game algorithm and update */
  if (player_turn == 1) {
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    delayMock(200, delay_adder);
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
    delayMock(200, delay_adder);
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    delayMock(200, delay_adder);
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
  }
  else if (player_turn == 2) {
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    delayMock(200, delay_adder);
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
    delayMock(200, delay_adder);
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    delayMock(200, delay_adder);
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
  }
}

/**
 * Will alternate the turn indicator LED for the winner
 *
 * @param winner: The player who won the game
 * @param pin_adder: The sum of the pin numbers being set
 * @param low_counter: The number of low sets
 * @param high_counter: The number of high sets
 * @param delay_adder: The sum of the delay time
 */
void IO_WinnerTurnIndicator(int winner, int &pin_adder, int &low_counter, int &high_counter, int &delay_adder) {
  if (winner == 1) {
    /* Set LED2 GPIO pin to low */
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    /* Alternate LED1 GPIO pin to indicate player 1 is winner */
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
    delayMock(1000, delay_adder);
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    delayMock(1000, delay_adder);
  }
  else {
    /* Set LED1 GPIO pin to low */
    digitalWriteMock(PLAYER1_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    /* Alternate LED2 GPIO pin to indicate player 2 is winner */
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH_MOCK, pin_adder, low_counter, high_counter);
    delayMock(1000, delay_adder);
    digitalWriteMock(PLAYER2_TURN_INDICATOR_LED_PIN, LOW_MOCK, pin_adder, low_counter, high_counter);
    delayMock(1000, delay_adder);
  }
}

/**
 * Initializes the game map LEDs
 *
 * @param function_called_correctly: Whether the function was called correctly or not
 * @param red_counter: How many times the red LC was called
 * @param blue_counter: How many times the blue LC was called
 * @param green_counter: How many times the green LC was called
 */
void IO_InitHWGameMap(bool &function_called_correctly, int &red_counter, int &blue_counter, int &green_counter) {
  /* Initialize the red LED max chip (via LED control) */
  function_called_correctly = lcShutdownMock(RED_MOCK, 0, false, red_counter, blue_counter, green_counter);
  if (function_called_correctly == true) {
    function_called_correctly = lcSetIntensityMock(RED_MOCK, 0, 15, red_counter, blue_counter, green_counter);
  }
  if (function_called_correctly == true) {
    function_called_correctly = lcClearDisplayMock(RED_MOCK, 0, red_counter, blue_counter, green_counter);
  }

  /* Initialize the blue LED max chip (via LED control) */
  if (function_called_correctly == true) {
    function_called_correctly = lcShutdownMock(BLUE_MOCK, 0, false, red_counter, blue_counter, green_counter);
  }
  if (function_called_correctly == true) {
    function_called_correctly = lcSetIntensityMock(BLUE_MOCK, 0, 15, red_counter, blue_counter, green_counter);
  }
  if (function_called_correctly == true) {
    function_called_correctly = lcClearDisplayMock(BLUE_MOCK, 0, red_counter, blue_counter, green_counter);
  }

  /* Initialize the green LED max chip (via LED control) */
  if (function_called_correctly == true) {
    function_called_correctly = lcShutdownMock(GREEN_MOCK, 0, false, red_counter, blue_counter, green_counter);
  }
  if (function_called_correctly == true) {
    function_called_correctly = lcSetIntensityMock(GREEN_MOCK, 0, 15, red_counter, blue_counter, green_counter);
  }
  if (function_called_correctly == true) {
    function_called_correctly = lcClearDisplayMock(GREEN_MOCK, 0, red_counter, blue_counter, green_counter);
  }
}

/**
 * Update the RGB LEDs corresponding to the game map
 *
 * @param mocked_board: The mocked game map board to use
 * @param function_called_correctly: Whether the function was called correctly
 * @param red_counter: How many times the red LC was called
 * @param blue_counter: How many times the blue LC was called
 * @param green_counter: How many times the green LC was called
 * @param true_counter: How many times the LEDs are set
 * @param row_adder: The sum of the rows set
 * @param col_adder: The sum of the cols set
 */
void IO_SetHWGameMap(int (&mocked_board)[8][8], bool &function_called_correctly, int &red_counter, int &blue_counter, int &green_counter, int &true_counter, int &row_adder, int &col_adder) {
  int max_row = -1;
  int max_col = -1;
  function_called_correctly = true;

  /* Get game map from game algorithm and update */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row + col) % 2 == 0) {
        /* Get MAX chip row and column */
        IO_MapToMaxChip(row, col, max_row, max_col);

        switch (mocked_board[row][col]) {
          case EMPTY_COLOR: /* No piece */
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(RED_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(GREEN_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(BLUE_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            break;
          case PLAYER1_COLOR: /* Player 1 regular piece */
            /* Red */
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(RED_MOCK, 0, max_row, max_col, true, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(GREEN_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(BLUE_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            break;
          case PLAYER2_COLOR: /* Player 2 regular piece */
            /* Blue */
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(RED_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(GREEN_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(BLUE_MOCK, 0, max_row, max_col, true, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            break;
          case PLAYER1_KING_COLOR: /* Player 1 king piece */
            /* Yellow */
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(RED_MOCK, 0, max_row, max_col, true, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(GREEN_MOCK, 0, max_row, max_col, true, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(BLUE_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            break;
          case PLAYER2_KING_COLOR: /* Player 2 king piece */
            /* Light Blue */
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(RED_MOCK, 0, max_row, max_col, false, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(GREEN_MOCK, 0, max_row, max_col, true, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            if (function_called_correctly == true) {
              function_called_correctly = lcSetLedMock(BLUE_MOCK, 0, max_row, max_col, true, red_counter, blue_counter, green_counter, true_counter, row_adder, col_adder);
            }
            break;
          default:
            break;
        }
      }
    }
  }
}
