/***********************************************************************************
 * @file MicrocontrollerProcess.ino
 * @brief The runnable file for the microcontroller to call all necessary functions
 ***********************************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"
#include "Io.h"

/**********************************
 ** Third party Library Includes
 **********************************/

/**********************************
 ** Global Variables
 **********************************/
/* Variable to indicate which player's turn it is */
int player_turn;

/* The saved game map */
int game_map[8][8];

/* Variables for storing potential moves */
String first_button_input; /* The string if there is only one button input */
String move_command[2]; /* The move command broken down into a string array */
String move_queue;
int move_int[2][2]; /* 2D array for storing the move to send to the game algorithm */

/* Indicate if there is a winner (0=No, 1=Yes) */
int winner;

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
  /* Pin setup */
  IO_InitButton();
  IO_InitTurnIndicator();

  /* Global variable initializations */
  first_button_input = "";
  move_command[0] = "";
  move_command[1] = "";
  move_queue = "";
  winner = 0;
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
        /* Store move in array */
        move_command[0] = first_button_input;
        move_command[1] = move_queue;
        first_button_input = "";
        move_queue = "";
      }
    }
    /* Clear the first button move (first_button_input) if a voice command gets received */
    else {
      first_button_input = "";
    }

    /* If there is a move command */
    if (move_command[0] != "" || move_command[1] != "") {
      /* Convert the string to a 2D integer array to send to the game algorithm */
      IO_ConvertMapToIndices(move_command, move_int);
  
      /* Make a call to the game algorithm to pass in moves */
      checkers_game.Checkers_Turn(move_int[0], move_int[1]);
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
