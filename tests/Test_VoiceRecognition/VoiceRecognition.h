/************************************************************
 * @file VoiceRecognition.h
 * @brief The header for the Voice Recognition algorithm
 * @note This file is copied over from src and modified for testing
 ************************************************************/
#ifndef VOICERECOGNITION_H
#define VOICERECOGNITION_H

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "Arduino.h"

/**********************************
 ** Function Prototypes
 **********************************/
/* Helper function to print (store for testing) string to output */
void VoiceRecognition_OutputError(String error, String &recent_error);

/* Helper function to parse moves into the correct format from received data */
void VoiceRecognition_ParseMoves(String moves, String &result);

/* Public functions in the source */
void VoiceRecognition_Init(bool &correct_functions_called, String &recent_error, int baud_rate, bool verbose_mode, bool factory_reset_en, bool factory_reset, bool echo, bool data);
void VoiceRecognition_GetInput(String &parsed_checker_move, bool connection, String input_data);

#endif /* VOICERECOGNITION_H */
