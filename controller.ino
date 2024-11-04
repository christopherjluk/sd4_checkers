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

void error(const __FlashStringHelper *err) {
  Serial.println(err);
  while (1);
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
    Serial.print("Entire data string: ");
    Serial.print(checkerMove);

  delay(1000);
}
