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
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Arduino.h"
#include "SPI.h"

/**********************************
 ** Defines
 **********************************/
/* Application settings */
#define FACTORYRESET_ENABLE      (1)
#define MINIMUM_FIRMWARE_VERSION ("0.6.6")
#define MODE_LED_BEHAVIOR        ("MODE")
#define VERBOSE_MODE             (true)

/**********************************
 ** Global Variables
 **********************************/
Adafruit_BluefruitLE_SPI ble(22, 17, 21);

/**********************************
 ** Private Function Prototypes
 **********************************/
void VoiceRecognition_OutputError(const __FlashStringHelper *err);
void VoiceRecognition_ParseMoves(String moves, String &result);

/**********************************
 ** Function Definitions
 **********************************/
/**
 * Catches any Bluetooth errors and puts in a looping state
 *
 * @param err: The error being passed in
 */
void VoiceRecognition_OutputError(const __FlashStringHelper *err) {
  Serial.println(err);
  while (1);
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
 */
void VoiceRecognition_Init() {
  /* Initializer serial */
  Serial.begin(115200);

  /* Initialize the Bluefruit module */
  if (!ble.begin(VERBOSE_MODE)) {
    VoiceRecognition_OutputError(F("Couldn't find Bluefruit, check wiring?"));
  }

  /* Optional factory reset */
  if (FACTORYRESET_ENABLE && !ble.factoryReset()) {
    VoiceRecognition_OutputError(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  /* Print Bluefruit info */
  ble.info();

  /* Set Bluefruit to DATA mode */
  ble.setMode(BLUEFRUIT_MODE_DATA);
}

/**
 * Receives checker move from BLE and parse to return to IO
 *
 * @param parsed_checker_move: The String that returns a checker move 
 */
void VoiceRecognition_GetInput(String &parsed_checker_move) {
  String checker_move = "";

  /* Check for data if BLE is connected */
  if (ble.isConnected()) {
    while (ble.available()) {
      /* Read and process incoming data */
      char received_data = ble.read();
      checker_move += received_data;

      /* Print the received data */
      Serial.print("Received Data: ");
      Serial.println(received_data);
    }
  }

  VoiceRecognition_ParseMoves(checker_move, parsed_checker_move);
  
  /* Print the parsed move */
  Serial.println(parsed_checker_move);
}
