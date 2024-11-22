/************************************************************
 * @file Io.h
 * @brief The header for I/O related functionalities
 ************************************************************/
#ifndef IO_H
#define IO_H

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "Arduino.h"

/**********************************
 ** Function Prototypes
 **********************************/
/* Helper function for converting board map to integer indices */
void IO_ConvertMapToIndices(String (&move_string)[2], int (&move_int)[2][2]);

/* Voice Recognition Input functions */
void IO_GetVoiceRecognitionInput(String (&move_command)[2]);

/* Button functions */
void   IO_InitButton();
String IO_GetButtonInput();

/* Turn Indicator LED functions */
void IO_InitTurnIndicator();
void IO_SetTurnIndicator(int player_turn);
void IO_BlinkTurnIndicator(int player_turn);
void IO_WinnerTurnIndicator(int winner);

/* Game Map LED functions */
void IO_InitHWGameMap();
void IO_SetHWGameMap(Checkers checker_game);

#endif /* IO_H */
