/************************************************************
 * @file VoiceRecognition.cpp
 * @brief The header for the Voice Recognition algorithm
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
void VoiceRecognition_Init();
void VoiceRecognition_CheckConnection();
void VoiceRecognition_GetInput(String (&parsed_checker_move)[2])

#endif /* VOICERECOGNITION_H */
