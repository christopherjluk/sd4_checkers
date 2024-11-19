/************************************************************
 * @file Io.h
 * @brief The header for I/O related functionalities
 * @note This file is copied over from src and modified for testing
 ************************************************************/
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
 ** Function Prototypes
 **********************************/
/* Helper function to converting board map to MAX chip coordinates */
void IO_MapToMaxChip(int row, int col, int &max_row, int &max_col);

/* Helper function for converting board map to integer indices */
void IO_ConvertMapToIndices(String (&move_string)[2], int (&move_int)[2][2]);

/* Voice Recognition Input functions */
void IO_GetVoiceRecognitionInput(String test_input, String (&move_command)[2]);

/* Button functions */
void   IO_InitButton(int &pin_adder, int &input_counter, int &output_counter, int &low_counter, int &high_counter);
String IO_GetButtonInput(int (&reading)[4], int &pin_adder, int &reading_adder, int &delay_adder);

/* Turn Indicator LED functions */
void IO_InitTurnIndicator(int &pin_adder, int &input_counter, int &output_counter);
void IO_SetTurnIndicator(int player_turn, int &pin_adder, int &low_counter, int &high_counter);
void IO_BlinkTurnIndicator(int player_turn, int &pin_adder, int &low_counter, int &high_counter, int &delay_adder);
void IO_WinnerTurnIndicator(int winner, int &pin_adder, int &low_counter, int &high_counter, int &delay_adder);

/* Game Map LED functions */
void IO_InitHWGameMap(bool &function_called_correctly, int &red_counter, int &blue_counter, int &green_counter);
void IO_SetHWGameMap(int (&mocked_board)[8][8], bool &function_called_correctly, int &red_counter, int &blue_counter, int &green_counter, int &true_counter, int &row_adder, int &col_adder);

#endif /* IO_H */