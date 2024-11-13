/************************************************************
 * @file Test_VoiceRecognition.ino
 * @brief The tests for the I/O module
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "VoiceRecognition.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "ArduinoUnit.h"

/**********************************
 ** Tests
 **********************************/
/**
 * VoiceRecognition_OutputError tests
 **/
test(VoiceRecognition_OutputError_Success) {
  String stored_error = "";
  VoiceRecognition_OutputError("Some error", stored_error);
  assertEqual(stored_error, "Some error");

  VoiceRecognition_OutputError("Another error", stored_error);
  assertEqual(stored_error, "Another error");
}

/**
 * VoiceRecognition_ParseMoves tests
 **/
test(VoiceRecognition_ParseMoves_Success) {
  String stored_result = "";
  VoiceRecognition_ParseMoves("A2 B3 C4", stored_result);
  assertEqual(stored_result, "A2 B3");

  VoiceRecognition_ParseMoves("E1 F5", stored_result);
  assertEqual(stored_result, "E1 F5");

  VoiceRecognition_ParseMoves("E12 F3F A2A", stored_result);
  assertEqual(stored_result, "E12 F3F");
}

/**
 * VoiceRecognition_Init tests
 **/
test(VoiceRecognition_Init_SerialBegin_Failure) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 9600, false, false, false, true, false);

  assertEqual(error, "");
  assertEqual(correct_functions_called, false);
}

test(VoiceRecognition_Init_BleBegin_Failure) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 115200, false, false, false, true, false);

  assertEqual(error, "Couldn't find Bluefruit, check wiring?");
  assertEqual(correct_functions_called, false);
}

test(VoiceRecognition_Init_BleFactoryResetEnable_Failure) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 115200, true, false, false, true, false);

  assertEqual(error, "Couldn't factory reset");
  assertEqual(correct_functions_called, false);
}

test(VoiceRecognition_Init_BleFactoryReset_Failure) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 115200, true, true, false, true, false);

  assertEqual(error, "Couldn't factory reset");
  assertEqual(correct_functions_called, false);
}

test(VoiceRecognition_Init_BleEcho_Failure) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 115200, true, true, true, true, false);

  assertEqual(error, "");
  assertEqual(correct_functions_called, false);
}

test(VoiceRecognition_Init_BleSetMode_Failure) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 115200, true, true, true, false, false);

  assertEqual(error, "");
  assertEqual(correct_functions_called, false);
}

test(VoiceRecognition_Init_Success) {
  String error = "";
  bool correct_functions_called = false;
  VoiceRecognition_Init(correct_functions_called, error, 115200, true, true, true, false, true);

  assertEqual(error, "");
  assertEqual(correct_functions_called, true);
}

/**
 *
 **/

/**********************************
 ** Function Definitions
 **********************************/
/**
 * Set up serial to receive test results
 *
 * @note Must be named "setup" so the MCU knows to run this first before running the loop
 */
void setup() {
  Serial.begin(115200);
  while(!Serial) {}
}

/**
 * Will loop through and run the tests, printing the results
 *
 * @note Must be named "loop" so it will repeatedly run on the MCU
 */
void loop() {
  Test::run();
}
