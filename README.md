# Voice Recognition LED Checkers Game
This Voice Recognition LED Checkers game was developed for the purposes of a Senior Design capstone project for ECE 1896: Senior Design Project at the University of Pittsburgh's Swanson School of Engineering. This project was designed in Fall 2024 by Senior Design Group ECE-4, consisting of team members: Grace Cullen, Christopher Erfort, Luke Finley, and Christopher Luk.

## Overview
The project is a fully functional checkers game played on a custom-designed printed circuit board. The inputs either come from a Bluetooth-compatible Voice Recognition app or from pressing buttons available on the board. The outputs, including the game map and turn indicators, are displayed via separate red, green, and blue LEDs. A switch is available to turn the circuit board on and off, powered by 4 connected AA batteries. An additional button is available to reset the game to the starting state when desired.

## How It Works
### Hardware
A printed circuit board was custom-designed for the purposes of storing all of the needed components for the game and wiring the components properly together. The components for the hardware needed include:
- Printed Circuit Board(s)
- 33 Red LEDs
- 32 Green LEDs
- 33 Blue LEDs
- 33 Buttons
- 4 110-Resistors
- 4 220-Resistors
- 4 330-Resistors
- 4 390-Resistors
- 4 510-Resistors
- 12 1K-Resistors
- 4 1.2K-Resistors
- 4 1.5K-Resistors
- 16 10K-Resistors
- 3 MAX7219 Chips
- ESP32 Dev Board

### Software
The software consists of a process making calls to a Game Algorithm module, an I/O module, and a Voice Recognition module. The process loops over and over in order for the game functionality to run properly.

#### Source
The MicrocontrollerProcess folder contains all of the files needed for the functionality to run on the board. The dependencies for this code is listed via the libraries in the `src/external` folder and can be downloaded directly in the Arduino IDE. In order to upload the code to the ESP32, you must press "Upload" in the Arduino IDE while in the `MicrocontrollerProcess.ino` file and verify the correct USB port and the ESP32 Dev Module is selected.

#### Tests
The tests folder contain all of the unit tests for the process and the different modules. These unit tests are run via the public `ArduinoUnit` unit testing library, which is included in the `tests/external` folder and can be downloaded directly in the Arduino IDE.

#### External
The external folder contains the code for the iOS app needed for the voice recognition app.
