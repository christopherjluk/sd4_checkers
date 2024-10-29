/**********************************
 ** Library Includes
 **********************************/
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

/* Strings for storing potential moves */
String first_button_input; /* The string if there is only one button input */
String move_command[2]; /* The move command to send to the game algorithm */
String move_queue;

/* Indicate if there is a winner (0=No, 1=Yes) */
int winner;

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
  player_turn = 1;
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
  if (winner == 0) { /* When there is no winner and the game is going on */
    if (first_button_input == ""){ /* If no buttons have already been pressed to indicate a move */
      IO_GetVoiceRecognitionInput(move_command);
    }
    if (move_command[0] == "" || move_command[1] == "") { /* If no voice command has been received */
      move_queue = IO_GetButtonInput();
      if (first_button_input == "" && move_queue != "") {
        /* Store first button input */
        first_button_input = move_queue;
      }
      else if (first_button_input != "" && move_queue != "") {
        /* Store move in array */
        move_command[0] = first_button_input;
        move_command[1] = move_queue;
        first_button_input = "";
      }
    }
    /* If one button input is received, store it until a second button input is received */
    if (move_command[0] != "" || move_command[1] != "") { /* If there is a move command */
      delay(5); /* temp */
      /* Make a call to the game algorithm to pass in moves */
    }
    IO_SetTurnIndicator(player_turn);
    IO_SetHWGameMap(game_map);
    /* Check if there is a winner */
  }
  else {
    IO_WinnerTurnIndicator(player_turn);
  }
}
