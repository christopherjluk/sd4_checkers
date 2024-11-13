/************************************************************
 * @file VoiceRecognition.cpp
 * @brief The implementation for the Voice Recognition algorithm
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "VoiceRecognition.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/

/**********************************
 ** Defines
 **********************************/

/**********************************
 ** Global Variables
 **********************************/

/**********************************
 ** Helper Functions
 **********************************/
/**
 * This function will mock a Serial.Begin call in non-testing functions (Expected baud rate is 115200)
 *
 * @param baud_rate: The passed in baud rate
 * @return bool: Whether the baud rate is 115200
 */
bool SerialBeginMock(int baud_rate) {
  if (baud_rate == 115200) {
    return true;
  }

  return false;
}

/**
 * This function will mock a BLE begin call
 *
 * @param verbose_mode: Whether the BLE will be set to verbose mode
 * @return bool: Wheteher the BLE is set in verbose mode or not
 */
bool BleBeginMock(bool verbose_mode) {
  if (verbose_mode == true) {
    return true;
  }

  return false;
}

/**
 * This function will mock a BLE factory reset call
 *
 * @param factory_reset: Whether the BLE is factory resetting
 * @return bool: Whether the BLE factory resets or not
 */
bool BleFactoryResetMock(bool factory_reset) {
  if (factory_reset == true) {
    return true;
  }

  return false;
}

/**
 * This function will mock a BLE echo call
 *
 * @param echo: The echo setting of the BLE
 * @return bool: Whether the BLE is not echoing or not
 */
bool BleEchoMock(bool echo) {
  if (echo == false) {
    return true;
  }

  return false;
}

/**
 * This function will mock a BLE info call
 *
 */
void BleInfoMock() {
  return;
}

/**
 * This function will mock a BLE set mode call
 *
 * @param data: The mode of the BLE (data mode = true, not data mode = false)
 * @return bool: Whether the BLE is set to data mode or not
 */
bool BleSetModeMock(bool data) {
  if (data == true) {
    return true;
  }

  return false;
}

/**
 * This function will mock a BLE IsConnected call
 *
 * @param connected: The connection status mock
 * @return bool: Whether the BLE is connected
 */
bool BleIsConnectedMock(bool connected) {
  if (connected == true) {
    return true;
  }

  return false;
}

/**
 * This function will mock a BLE available call
 *
 * @param input: The input to check whether there is still a string to parse through
 * @return bool: Whether there is data to parse through
 */
bool BleAvailableMock(String input) {
  if (input.length() == 0) {
    return false;
  }

  return true;
}

char BleReadMock(String &input) {
  char ret_val;

  if (input.length() != 0) {
    ret_val = input.charAt(0);
    input.remove(0, 1);
  }

  return ret_val;
}

/**********************************
 ** Function Definitions
 **********************************/
/**
 * Catches any Bluetooth errors and puts in a looping state
 *
 * @param error: The error being passed in
 * @param recent_error: The place to store the most recent error
 */
void VoiceRecognition_OutputError(String error, String &recent_error) {
  recent_error = error;
}

/**
 * Parses a command from the BLE application into a move array of length 2
 *
 * @param moves: The moves from the BLE application to parse
 * @param result: The result of the move to send back
 */
void VoiceRecognition_ParseMoves(String moves, String &result) {
  /* Vector to store parsed moves */
  int index = 0;
  int start = 0;
  int space_index = moves.indexOf(' '); /* Find the first space */

  /* Only stores the first five characters and parses for spaces */
  while ((space_index != -1) && (index < 1)) {
    /* Get the substring from start to the next space */
    if (start != space_index) {
      result = moves.substring(start, moves.indexOf(' ', space_index + 1));
      index += 1;
    }

    /* Move to the next space */
    start = space_index + 1;
    space_index = moves.indexOf(' ', start);
  }
}

/**
 * Initializes the voice recognition module, sets up serial and Bluetooth
 *
 * @param correct_functions_called: A boolean to ensure all functions have been called correctly
 * @param recent_error: The string of the most recent error
 * @param baud_rate: The mocked baud rate to set up the serial
 * @param verbose_mode: The mocked verbose mode to set up the serial
 * @param factory_reset_en: The factory reset enable mock
 * @param factory_reset: The factory reset mock
 * @param echo: The echo setting mock
 * @param data: The data mode setting mock
 */
void VoiceRecognition_Init(bool &correct_functions_called, String &recent_error, int baud_rate, bool verbose_mode, bool factory_reset_en, bool factory_reset, bool echo, bool data) {
  /* Initializer serial */
  correct_functions_called = SerialBeginMock(baud_rate);

  /* Initialize the Bluefruit module */
  if (correct_functions_called == true) {
    if (!BleBeginMock(verbose_mode)) {
      VoiceRecognition_OutputError("Couldn't find Bluefruit, check wiring?", recent_error);
      correct_functions_called = false;
    }
  }

  /* Optional factory reset */
  if (correct_functions_called == true) {
    if (factory_reset_en == false || !BleFactoryResetMock(factory_reset)) {
      VoiceRecognition_OutputError("Couldn't factory reset", recent_error);
      correct_functions_called = false;
    }
  }

  /* Disable command echo from Bluefruit */
  if (correct_functions_called == true) {
    correct_functions_called = BleEchoMock(echo);
  }

  /* Print Bluefruit info */
  BleInfoMock();

  /* Set Bluefruit to DATA mode */
  if (correct_functions_called == true) {
    correct_functions_called = BleSetModeMock(data);
  }
}

/**
 * Receives checker move from BLE and parse to return to IO
 *
 * @param parsed_checker_move: The String that returns a checker move
 * @param connection: The mocked BLE connection status
 */
void VoiceRecognition_GetInput(String &parsed_checker_move, bool connection, String input_data) {
  String checker_move = "";
  String input_reading = input_data;

  /* Check for data if BLE is connected */
  if (BleIsConnectedMock(connection)) {
    while (BleAvailableMock(input_reading)) {
      /* Read and process incoming data */
      char received_data = BleReadMock(input_reading);
      checker_move += received_data;
    }
  }

  VoiceRecognition_ParseMoves(checker_move, parsed_checker_move);
}
