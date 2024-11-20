/***********************************************************************************
 * @file MicrocontrollerProcess.ino
 * @brief The runnable file for the microcontroller to call all necessary functions
 * @note Libraries needed - Adafruit_BluefruitLE_nRF51, ArduinoSTL, EspSoftwareSerial, LedControl
 ***********************************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"
#include "Io.h"
#include "VoiceRecognition.h"

/**********************************
 ** Third party Library Includes
 **********************************/
#include "Arduino.h"

/**********************************
 ** Global Variables
 **********************************/
/* Variables for storing potential moves */
String first_button_input; /* The string if there is only one button input */
String move_command[2]; /* The move command broken down into a string array */
String move_queue;
int move_int[2][2]; /* 2D array for storing the move to send to the game algorithm */
int valid_move;

/* The Checkers game containing the board and player information */
Checkers checkers_game;

/**********************************
 ** Function Definitions
 **********************************/
/**
 * Initialize any variables if it hasn't been done already and initialize MCU, makes initial calls
 *
 * @note Must be named "setup" so the MCU knows to run this first before running the loop
 */
void setup() {
  /* Voice recognition setup */
  VoiceRecognition_Init();

  /* Pin setup */
  IO_InitButton();
  IO_InitTurnIndicator();
  IO_InitHWGameMap();

  /* Global variable initializations */
  first_button_input = "";
  move_command[0] = "";
  move_command[1] = "";
  move_queue = "";
}

/**
 * Will loop through the processes and update modules accordingly
 *
 * @note Must be named "loop" so it will repeatedly run on the MCU
 */
void loop() {
  /* Check if there is a winner: when there is no winner, the game goes on */
  if (checkers_game.Checkers_GetWin() == 0) {
    /* Check the voice recognition module for a move */
    if (first_button_input == ""){
      IO_GetVoiceRecognitionInput(move_command);
    }

    /* If no voice command has been received */
    if (move_command[0] == "" || move_command[1] == "") {
      move_queue = IO_GetButtonInput();
      if (first_button_input == "" && move_queue != "") {
        /* Store first button input */
        first_button_input = move_queue;
        move_queue = "";
      }
      else if (first_button_input != "" && move_queue != "") {
        /* If read button is the same as the first move, ignore as debouncing may not be detected yet */
        if (move_queue != first_button_input) {
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
      IO_ConvertMapToIndices(move_command, move_int);
  
      /* Make a call to the game algorithm to pass in moves */
      valid_move = checkers_game.Checkers_Turn(move_int[0], move_int[1]);

      /* Blink the turn indicator LED if the move is invalid */
      if (valid_move == 0) {
        IO_BlinkTurnIndicator(checkers_game.Checkers_GetActivePlayer());
      }
    }

    /* Set the turn indicator LEDs */
    IO_SetTurnIndicator(checkers_game.Checkers_GetActivePlayer());

    /* Set the game map LEDs */
    IO_SetHWGameMap(checkers_game);
  }
  else {
    /* Flash the turn indicator LED based on the winner until restarted */
    IO_WinnerTurnIndicator(checkers_game.Checkers_GetActivePlayer());
  }
}
