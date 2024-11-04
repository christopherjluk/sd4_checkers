/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules
*********************************************************************/

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"



//the above step needs to happen sequentially to get data to the esp32
//look for a peripheral to connect to
//connect, record audio
//determine if input is valid
//check for if the hardcoded service and characteristic are offered by ble module
//send data to the characteristic
//ble.read()

/* Application settings */
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"


//cs to gpio 22
//irq to gpio 17
//rst to gpio 21

//vin to 3v
//grnd to grnd
//miso to gpio 19
//mosi to gpio 23
//sck to gpio18

// Define the BLE SPI object
Adafruit_BluefruitLE_SPI ble(22, 17, 21);
String checkerMove;

String test[8];


void error(const __FlashStringHelper *err) {
  Serial.println(err);
  while (1);
}

//make all array values null (not set)
void clearArray(String arr[]){

 for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); i++) {
  if (test[i].length() == 0) {
    break; // Exit the loop if the element is empty
  }

  test[i]="";
 }

}


//parse function
//separate checker move into an array where each element is a string of 5 characters
//send to Chris L's function that converts each element to the format of Luke's function
void parseMoves(String moves,String result[]) {
 // Vector to store parsed moves
  int i=0;
  int start = 0;
  int spaceIndex = moves.indexOf(' '); // Find the first space

  while (spaceIndex != -1) {
    // Get the substring from start to the next space
    if (start != spaceIndex) {
      result[i]=moves.substring(start, moves.indexOf(' ', spaceIndex + 1));
      i+=1;
    }
    // Move to the next space
    start = spaceIndex + 1;
    spaceIndex = moves.indexOf(' ', start);
  }
}

void setup(void) {
  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit App Controller Example"));

  /* Initialize the Bluefruit module */
  if (!ble.begin(VERBOSE_MODE)) {
    error(F("Couldn't find Bluefruit, check wiring?"));
  }
  Serial.println(F("OK!"));

  /* Optional factory reset */
  if (FACTORYRESET_ENABLE) {
    if (!ble.factoryReset()) {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  /* Print Bluefruit info */
  ble.info();

  /* Wait for connection */
  Serial.println(F("Waiting for BLE connection..."));
  while (!ble.isConnected()) {
    delay(500);
  }
  Serial.println(F("Connected!"));

  /* Set Bluefruit to DATA mode */
  ble.setMode(BLUEFRUIT_MODE_DATA);
  Serial.println("loop is next");
}

void loop(void) {
  // Check if data is available from the BLE module

  Serial.println();
  if(ble.isConnected()){
    Serial.println("Connected");
  }
  else {
    Serial.println("Not connected");
  }

  checkerMove="";
  while (ble.available()) {
    // Read and process incoming data
    Serial.println("BLE available");
    char receivedData = ble.read();
    checkerMove+=receivedData;
    
    // Print the received data
    Serial.print("Received Data: ");
    Serial.println(receivedData);

  }

    parseMoves(checkerMove,test);

for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); i++) {
  if (test[i].length() == 0) {
    break; // Exit the loop if the element is empty
  }

  Serial.println("Move " + String(i) + ": " + test[i]);
}

  clearArray(test);

  delay(1000);
}




