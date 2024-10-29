#ifndef IO_H
#define IO_H

/**********************************
 ** Library Includes
 **********************************/

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "Arduino.h"

/**********************************
 ** Function prototypes
 **********************************/
/* Voice Recognition Input functions */
void IO_GetVoiceRecognitionInput(String (&move_command)[2]);

/* Button functions */
void   IO_InitButton();
String IO_GetButtonInput();

/* Turn Indicator LED functions */
void IO_InitTurnIndicator();
void IO_SetTurnIndicator(int player_turn);
void IO_WinnerTurnIndicator(int winner);

/* Game Map LED functions */
void IO_SetHWGameMap(int (&game_map)[8][8]);

#endif // IO_H