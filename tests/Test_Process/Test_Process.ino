/************************************************************
 * @file Test_Process.ino
 * @brief The tests for the microcontroller process
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "ArduinoUnit.h"

/**********************************
 ** Global Variables
 **********************************/
/* Variables for storing potential moves */
String first_button_input; /* The string if there is only one button input */
String move_command[2]; /* The move command broken down into a string array */
String move_queue;
int move_int[2][2]; /* 2D array for storing the move to send to the game algorithm */
int valid_move;

/**********************************
 ** Helper Functions
 **********************************/
/**
 * This function mocks VoiceRecognition_Init in the process
 *
 */
void VoiceRecognitionInitMock() {
  return;
}

/**
 * This function mocks IO_InitButton in the process
 *
 */
void IOInitButtonMock() {
  return;
}

/**
 * This function mocks IO_InitTurnIndicator in the process
 *
 */
void IOInitTurnIndicatorMock() {
  return;
}

/**
 * This function mocks IO_InitHWGameMap in the process
 *
 */
void IOInitHwGameMapMock() {
  return;
}

/**
 * This function mocks IO_GetVoiceRecognitionInput in the process
 *
 * @param move: The move to pass in
 * @return String: The move received
 */
String IOGetVoiceRecognitionInputMock(String move) {
  return move;
}

/**
 * This function mocks IO_GetButtonInput in the process
 *
 * @param move: The move to pass in
 * @return String: The move received
 */
String IOGetButtonInputMock(String move) {
  return move;
}

/**
 * Converts a move command string to a form of a 2d integer array
 *
 * @param move_string: The move command string being converted
 * @param move_int: The move command returned as a form of a 2d integer array
 */
void IOConvertMapToIndicesMock(String (&move_string)[2], int (&move_int)[2][2]) {
  move_int[0][0] = move_string[0].charAt(0) - 65; /* A is 65 in the ASCII code, corresponds with row 0 */
  move_int[0][1] = move_string[0].charAt(1) - 49; /* 1 is 49 in the ASCII code, corresponds with column 0 */
  move_int[1][0] = move_string[1].charAt(0) - 65; /* A is 65 in the ASCII code, corresponds with row 0 */
  move_int[1][1] = move_string[1].charAt(1) - 49; /* 1 is 49 in the ASCII code, corresponds with column 0 */
}

/**
 * This function mocks Checkers_Turn in the process
 *
 * @param from: Where to move the piece from
 * @param to: Where to move the piece to
 * @return int: 1 if the move was in bounds, 0 if not
 */
int CheckersTurnMock(int from[2], int to[2]) {
  if (from[0] >= 0 && from[0] < 8 && from[1] >= 0 && from[1] < 8 && to[0] >= 0 && to[0] < 8 && to[1] >= 0 && to[1] < 8) {
    return 1;
  }

  return 0;
}

/**
 * This function mocks Checkers_GetActivePlayer in the process
 *
 * @param active_player: The active player to pass in
 * @return int: The active player returned
 */
int CheckersGetActivePlayerMock(int active_player) {
  return active_player;
}

/**
 * This function mocks IO_SetTurnIndicator in the process
 *
 * @param active_player: The active player to pass in
 */
void IOSetTurnIndicatorMock(int active_player) {
  return;
}

/**
 * This function mocks IO_BlinkTurnIndicator in the process
 *
 * @param active_player: The active player to pass in
 */
void IOBlinkTurnIndicatorMock(int active_player) {
  return;
}

/**
 * This function mocks IO_WinnerTurnIndicator in the process
 *
 * @param active_player: The active player to pass in
 * @return int: The winner
 */
int IOWinnerTurnIndicatorMock(int active_player) {
  return active_player;
}

/**
 * This function mocks IO_SetHWGameMap in the process
 *
 */
void IOSetHwGameMapMock() {
  return;
}

/**********************************
 ** Functions to Test
 **********************************/
/**
 * Initialize any variables if it hasn't been done already and initialize MCU, makes initial calls
 *
 */
void Process_Setup() {
  /* Voice recognition setup */
  VoiceRecognitionInitMock();

  /* Pin setup */
  IOInitButtonMock();
  IOInitTurnIndicatorMock();
  IOInitHwGameMapMock();

  /* Global variable initializations */
  first_button_input = "";
  move_command[0] = "";
  move_command[1] = "";
  move_queue = "";
}

/**
 * Will loop through the processes and update modules accordingly
 *
 * @param win: Whether there is a winner in the game, mocking Checkers_GetWin
 * @param voice_rec: Whether to use the voice recognition or the button
 * @param move: The mocked received move
 * @param active_player: The mocked active player
 * @param winner: The winner of the game (if there is one)
 */
void Process_Loop(bool win, bool voice_rec, String (&move)[2], int active_player, int &winner) {
  /* Check if there is a winner: when there is no winner, the game goes on */
  if (win == false) {
    /* Check the voice recognition module for a move */
    if (first_button_input == "" && voice_rec == true){
      move_command[0] = IOGetVoiceRecognitionInputMock(move[0]);
      move_command[1] = IOGetVoiceRecognitionInputMock(move[1]);
    }

    /* If no voice command has been received */
    if ((move_command[0] == "" || move_command[1] == "") && voice_rec == false) {
      move_queue = IOGetButtonInputMock(move[0]);
      if (first_button_input == "" && move_queue != "") {
        /* Store first button input */
        first_button_input = move_queue;
        move_queue = "";
      }

      move_queue = IOGetButtonInputMock(move[1]);
      if (first_button_input != "" && move_queue != "") {
        /* If read button is the same as the first move, ignore as debouncing may not be detected yet */
        if (first_button_input != move_queue) {
          /* Store move in array */
          move_command[0] = first_button_input;
          move_command[1] = move_queue;
          first_button_input = "";
          move_queue = "";
        }
      }
    }
    /* Clear the first button move (first_button_input) if a voice command gets received */
    else {
      first_button_input = "";
    }

    /* If there is a move command */
    if (move_command[0] != "" && move_command[1] != "") {
      /* Convert the string to a 2D integer array to send to the game algorithm */
      IOConvertMapToIndicesMock(move_command, move_int);
  
      /* Make a call to the game algorithm to pass in moves */
      valid_move = CheckersTurnMock(move_int[0], move_int[1]);

      /* Blink the turn indicator LED if the move is invalid */
      if (valid_move == 0) {
        IOBlinkTurnIndicatorMock(CheckersGetActivePlayerMock(active_player));
      }
    }

    /* Set the turn indicator LEDs */
    IOSetTurnIndicatorMock(CheckersGetActivePlayerMock(active_player));

    /* Set the game map LEDs */
    IOSetHwGameMapMock();
  }
  else {
    /* Flash the turn indicator LED based on the winner until restarted */
    winner = IOWinnerTurnIndicatorMock(CheckersGetActivePlayerMock(active_player));
  }
}

/**********************************
 ** Tests
 **********************************/
/**
 * Process_Setup tests
 **/
test(Process_Setup_Success) {
  Process_Setup();

  assertEqual(first_button_input, "");
  assertEqual(move_command[0], "");
  assertEqual(move_command[1], "");
  assertEqual(move_queue, "");
}

/**
 * Process_Loop tests
 **/
test(Process_Loop_VoiceRec_Success) {
  int winner = 0;
  String move_command[2] = {"A1", "B2"};

  Process_Loop(false, true, move_command, 1, winner);
  assertEqual(winner, 0);
}

test(Process_Loop_ButtonInput_Different_Success) {
  int winner = 0;
  String move_command[2] = {"A1", "B2"};

  Process_Loop(false, false, move_command, 1, winner);
  assertEqual(winner, 0);
}

test(Process_Loop_ButtonInput_Same_Success) {
  int winner = 0;
  String move_command[2]  = {"A1", "A1"};

  Process_Loop(false, false, move_command, 1, winner);
  assertEqual(winner, 0);
}

test(Process_Loop_InvalidMove_Success) {
  int winner = 0;
  String move_command[2] = {"D9", "I3"};

  Process_Loop(false, false, move_command, 1, winner);
  assertEqual(winner, 0);
}

test(Process_Loop_Player1Win_Success) {
  int winner = 0;
  String move_command[2] = {"A1", "B2"};

  Process_Loop(true, false, move_command, 1, winner);
  assertEqual(winner, 1);
}

test(Process_Loop_Player2Win_Success) {
  int winner = 0;
  String move_command[2] = {"A1", "B2"};

  Process_Loop(true, false, move_command, 2, winner);
  assertEqual(winner, 2);
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
