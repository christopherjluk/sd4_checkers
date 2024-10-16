/* Includes */

/* Pins (GPIO needs to be finalized) */
#define PLAYER1_TURN_INDICATOR_LED_PIN (2)
#define PLAYER2_TURN_INDICATOR_LED_PIN (15)
#define BUTTON_ARRAY_PIN1 (1)
#define BUTTON_ARRAY_PIN2 (3)
#define BUTTON_ARRAY_PIN3 (4)
#define BUTTON_ARRAY_PIN4 (5)
#define LED_MAX_CHIP_RED_PIN (6)
#define LED_MAX_CHIP_GREEN_PIN (7)
#define LED_MAX_CHIP_BLUE_PIN (8)

/* Colors */
#define EMPTY_COLOR (0)
#define PLAYER1_COLOR (1)
#define PLAYER2_COLOR (2)
#define PLAYER1_KING_COLOR (3)
#define PLAYER2_KING_COLOR (4)

/* Button array thresholds */
#define BUTTON_THRESHOLD1 (5)
#define BUTTON_THRESHOLD2 (10)
#define BUTTON_THRESHOLD3 (15)
#define BUTTON_THRESHOLD4 (20)
#define BUTTON_THRESHOLD5 (25)
#define BUTTON_THRESHOLD6 (30)
#define BUTTON_THRESHOLD7 (35)
#define BUTTON_THRESHOLD8 (40)
#define ANALOG_READ_MAX (4096)

/* Global variables */
int player_turn; /* Indicates which player's turn it is */
int game_map[8][8]; /* The saved game map */
int color; /* The color mapping */
String first_button_input; /* The string if there is only one button input */
String move_command[2]; /* The move command to send to the game algorithm */
int red_data; /* Data to send out to the MAX chip controlling red */
int blue_data; /* Data to send out to the MAX chip controlling blue */
int green_data; /* Data to send out to the MAX chip controlling green */
int winner; /* Indicate if there is a winner (and number will indicate which player) */

/**
 * Initialize any variables if it hasn't been done already and initialize MCU, makes initial calls
 *
 * @note Must be named "setup" so the MCU knows to run this first before running the loop
 */
void setup() {
  /* Pin setup */
  pinMode(PLAYER1_TURN_INDICATOR_LED_PIN, OUTPUT);
  pinMode(PLAYER2_TURN_INDICATOR_LED_PIN, OUTPUT);
  pinMode(BUTTON_ARRAY_PIN1, INPUT);
  pinMode(BUTTON_ARRAY_PIN2, INPUT);
  pinMode(BUTTON_ARRAY_PIN3, INPUT);
  pinMode(BUTTON_ARRAY_PIN4, INPUT);
  pinMode(LED_MAX_CHIP_RED_PIN, OUTPUT);
  pinMode(LED_MAX_CHIP_BLUE_PIN, OUTPUT);
  pinMode(LED_MAX_CHIP_GREEN_PIN, OUTPUT);

  /* Global variable initializations */
  player_turn = 1;
  first_button_input = "";
  move_command[0] = "";
  move_command[1] = "";
  winner = 0;
}

/**
 * Will loop through the processes and update modules accordingly
 *
 * @note Must be named "loop" so it will repeatedly run on the MCU
 */
void loop() {
  if (winner == 0) { /* When there is no winner and the game is going on */
    if (first_button_input == ""){ /* If no buttons have already been pressed to indicate a move */
      check_for_voice_command_bt();
    }
    if (move_command[0] == "" || move_command[1] == "") { /* If no voice command has been received */
      check_for_button_input();
    }
    /* If one button input is received, store it until a second button input is received */
    if (move_command[0] != "" || move_command[1] != "") { /* If there is a move command */
      delay(5); /* temp */
      /* Make a call to the game algorithm to pass in moves */
    }
    update_turn_indicator();
    update_hw_game_map();
    /* Check if there is a winner */
  }
  else {
    winning_indicator();
  }
}

/**
 * Will check for a voice command and make sure it follows the right format
 *
 * @note Update this function to return a move command
 */
void check_for_voice_command_bt() {
  String voice_command = ""; /* Get voice command from Voice Recognition module */

  /* Voice commands should be in the format of A1 B2 */
  if ((voice_command.charAt(0) >= 'A' && voice_command.charAt(0) <= 'F') &&
      (voice_command.charAt(1) >= '1' && voice_command.charAt(1) <= '8') &&
      (voice_command.charAt(2) == ' ') &&
      (voice_command.charAt(3) >= 'A' && voice_command.charAt(3) <= 'F') &&
      (voice_command.charAt(4) >= '1' && voice_command.charAt(4) <= '8')) {
    /* Do something */
    move_command[0] = voice_command.substring(0, 2); /* Location to move from */
    move_command[1] = voice_command.substring(3, 2); /* Location to move to */
  }
}

/**
 * Will check if any buttons have been pressed
 *
 * @note Update this function to return a partial move command
 */
void check_for_button_input() {
  String move_queue = "";
  
  /* Button array 1 for rows 1 and 2 */
  if (analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD1) {
    /* Button for [1, 1] */
    move_queue = "A1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [1, 3] */
    move_queue = "A3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [1, 5] */
    move_queue = "A5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [1, 7] */
    move_queue = "A7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [2, 2] */
    move_queue = "B2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [2, 4] */
    move_queue = "B4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [2, 6] */
    move_queue = "B6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN1) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [2, 8] */
    move_queue = "B8";
  }

  /* Button array 2 for rows 3 and 4 */
  if (analogRead(BUTTON_ARRAY_PIN2) < BUTTON_THRESHOLD1) {
    /* Button for [3, 1] */
    move_queue = "C1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [3, 3] */
    move_queue = "C3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [3, 5] */
    move_queue = "C5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [3, 7] */
    move_queue = "C7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [4, 2] */
    move_queue = "D2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [4, 4] */
    move_queue = "D4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [4, 6] */
    move_queue = "D6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN2) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [4, 8] */
    move_queue = "D8";
  }

  /* Button array 3 for rows 5 and 6 */
  if (analogRead(BUTTON_ARRAY_PIN3) < BUTTON_THRESHOLD1) {
    /* Button for [5, 1] */
    move_queue = "E1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [5, 3] */
    move_queue = "E3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [5, 5] */
    move_queue = "E5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [5, 7] */
    move_queue = "E7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [6, 2] */
    move_queue = "F2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [6, 4] */
    move_queue = "F4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [6, 6] */
    move_queue = "F6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN3) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [6, 8] */
    move_queue = "F8";
  }
  
  /* Button array 4 for rows 7 and 8 */
  if (analogRead(BUTTON_ARRAY_PIN4) < BUTTON_THRESHOLD1) {
    /* Button for [7, 1] */
    move_queue = "G1";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD1 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD2) {
    /* Button for [7, 3] */
    move_queue = "G3";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD2 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD3) {
    /* Button for [7, 5] */
    move_queue = "G5";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD3 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD4) {
    /* Button for [7, 7] */
    move_queue = "G7";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD4 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD5) {
    /* Button for [8, 2] */
    move_queue = "H2";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD5 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD6) {
    /* Button for [8, 4] */
    move_queue = "H4";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD6 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD7) {
    /* Button for [8, 6] */
    move_queue = "H6";
  }
  else if (analogRead(BUTTON_ARRAY_PIN4) > BUTTON_THRESHOLD7 && analogRead(BUTTON_ARRAY_PIN1) < BUTTON_THRESHOLD8) {
    /* Button for [8, 8] */
    move_queue = "H8";
  }

  /* Verify only one output pin is getting one accepted analog reading at a time */
  if (analogRead(BUTTON_ARRAY_PIN1) < ANALOG_READ_MAX && analogRead(BUTTON_ARRAY_PIN2) < ANALOG_READ_MAX &&
      analogRead(BUTTON_ARRAY_PIN3) < ANALOG_READ_MAX && analogRead(BUTTON_ARRAY_PIN4) < ANALOG_READ_MAX) {
    move_queue = "";
  }
  
  if (first_button_input == "" && move_queue != "") {
    /* Store first button input */
    first_button_input = move_queue;
  }
  else if (first_button_input != "" && move_queue != "") {
    /* Store move in array */
    move_command[0] = first_button_input;
    move_command[1] = move_queue;
    first_button_input = "";
  }
}

/**
 * Will update the LED indicating the turn indicators throughout each process
 *
 */
void update_turn_indicator() {
  /* Get player turn from game algorithm and update */
  if (player_turn == 1) {
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH); /* Set LED1 GPIO pin to high */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);  /* Set LED2 GPIO pin to low */
  }
  else if (player_turn == 2) {
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);  /* Set LED1 GPIO pin to low */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH); /* Set LED2 GPIO pin to high */
  }
}

/**
 * Will update the RGB LEDs corresponding to the game map
 *
 */
void update_hw_game_map() {
  /* Get game map from game algorithm and update */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      switch (color) {
        case EMPTY_COLOR: /* No piece */
          /* Set RGB LED[row][col] to off */
          break;
        case PLAYER1_COLOR: /* Player 1 regular piece */
          /* Set RGB LED[row][col] to color 1 */
          break;
        case PLAYER2_COLOR: /* Player 2 regular piece */
          /* Set RGB LED[row][col] to color 1 */
          break;
        case PLAYER1_KING_COLOR: /* Player 1 king piece */
          /* Set RGB LED[row][col] to color 1 */
          break;
        case PLAYER2_KING_COLOR: /* Player 2 king piece */
          /* Set RGB LED[row][col] to color 1 */
          break;
        default:
          break;
      }
    }
  }
}

/**
 * Will turn on and off the turn indicator LED depending on the winner
 *
 */
void winning_indicator() {
  if (winner == 1) {
    /* Set LED2 GPIO pin to low */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);
    /* Alternate LED1 GPIO pin to indicate player 1 is winner */
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, HIGH);
    delay(100);
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);
    delay(100);
  }
  else {
    /* Set LED1 GPIO pin to low */
    digitalWrite(PLAYER1_TURN_INDICATOR_LED_PIN, LOW);
    /* Alternate LED2 GPIO pin to indicate player 2 is winner */
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, HIGH);
    delay(100);
    digitalWrite(PLAYER2_TURN_INDICATOR_LED_PIN, LOW);
    delay(100);
  }
}
