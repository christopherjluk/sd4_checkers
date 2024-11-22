/************************************************************
 * @file Test_Checkers.ino
 * @brief The tests for the game algorithm module
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"

/**********************************
 ** Third Party Libraries Includes
 **********************************/
#include "ArduinoUnit.h"

/**********************************
 ** Tests
 **********************************/
/**
 * Checkers constructor tests
 **/
test(Checkers_Constructor_Success) {
  Checkers checkers_game;

  /* Check initial values */
  assertEqual(checkers_game.Checkers_GetP1Count(), 12);
  assertEqual(checkers_game.Checkers_GetP2Count(), 12);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 1);
  assertEqual(checkers_game.jump_lock[2], 0);
  assertEqual(checkers_game.Checkers_GetWin(), 0);
  
  /* Check the game board */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row > 4) && ((row + col) % 2 == 0)) {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 1);
      }
      else if ((row < 3) && ((row + col) % 2 == 0)) {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 2);
      }
      else {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 0);
      }
    }
  }
}

/**
 * Checkers_GetBoardAt tests
 **/
test(Checkers_GetBoardAt_AllZeros_Success) {
  Checkers checkers_game;

  /* Set the board to 0s */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      checkers_game.board[row][col] = 0;
    }
  }

  /* Check the game board */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 0);
    }
  }
}

test(Checkers_GetBoardAt_Alternating_Success) {
  Checkers checkers_game;

  /* Set the board to 0s */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row + col) % 2 == 0) {
        checkers_game.board[row][col] = 1;
      }
      else {
        checkers_game.board[row][col] = 2;
      }
    }
  }

  /* Check the game board */
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row + col) % 2 == 0) {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 1);
      }
      else {
        assertEqual(checkers_game.Checkers_GetBoardAt(row, col), 2);
      }
    }
  }
}

/**
 * Checkers_GetP1Count tests
 **/
test(Checkers_GetP1Count_Success) {
  Checkers checkers_game;
  assertEqual(checkers_game.Checkers_GetP1Count(), 12);

  checkers_game.p1_count = 8;
  assertEqual(checkers_game.Checkers_GetP1Count(), 8);

  checkers_game.p1_count = 4;
  assertEqual(checkers_game.Checkers_GetP1Count(), 4);

  checkers_game.p1_count = 0;
  assertEqual(checkers_game.Checkers_GetP1Count(), 0);
}

/**
 * Checkers_GetP2Count tests
 **/
test(Checkers_GetP2Count_Success) {
  Checkers checkers_game;
  assertEqual(checkers_game.Checkers_GetP2Count(), 12);

  checkers_game.p2_count = 8;
  assertEqual(checkers_game.Checkers_GetP2Count(), 8);

  checkers_game.p2_count = 4;
  assertEqual(checkers_game.Checkers_GetP2Count(), 4);

  checkers_game.p2_count = 0;
  assertEqual(checkers_game.Checkers_GetP2Count(), 0);
}

/**
 * Checkers_GetActivePlayer tests
 **/
test(Checkers_GetActivePlayer_Success) {
  Checkers checkers_game;
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 1);

  checkers_game.active_player = 2;
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 2);
}

/**
 * Checkers_GetWin tests
 **/
test(Checkers_GetWin_Success) {
  Checkers checkers_game;
  assertEqual(checkers_game.Checkers_GetWin(), 0);

  checkers_game.won = 1;
  assertEqual(checkers_game.Checkers_GetWin(), 1);
}

/**
 * Checkers_HasMove tests
 **/
test(Checkers_HasMove_HasMove_Success) {
  Checkers checkers_game;
  assertEqual(checkers_game.Checkers_HasMove(), true);
}

test(Checkers_HasMove_NoMove_Success) {
  Checkers checkers_game;

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row > 4) && ((row + col) % 2 == 0)) {
        checkers_game.board[row][col] = 0;
      }
    }
  }

  checkers_game.board[3][1] = 1;
  checkers_game.active_player = 2;

  assertEqual(checkers_game.Checkers_HasMove(), false);
}

/**
 * Checkers_TurnOver tests
 **/
test(Checkers_TurnOver_Player1_Regular_NW_Success) {
  Checkers checkers_game;
  int move_to[2] = {5, 3};
  checkers_game.board[4][2] = 2;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[4][2] = 4;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player1_Regular_NE_Success) {
  Checkers checkers_game;
  int move_to[2] = {5, 3};
  checkers_game.board[4][4] = 2;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[4][4] = 4;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player1_King_NW_Success) {
  Checkers checkers_game;
  int move_to[2] = {5, 3};
  checkers_game.board[5][3] = 3;
  checkers_game.board[4][2] = 2;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[4][2] = 4;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player1_King_NE_Success) {
  Checkers checkers_game;
  int move_to[2] = {5, 3};
  checkers_game.board[5][3] = 3;
  checkers_game.board[4][4] = 2;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[4][4] = 4;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player1_King_SW_Success) {
  Checkers checkers_game;
  int move_to[2] = {2, 2};
  checkers_game.board[2][2] = 3;
  checkers_game.board[3][1] = 2;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[3][1] = 4;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player1_King_SE_Success) {
  Checkers checkers_game;
  int move_to[2] = {2, 2};
  checkers_game.board[2][2] = 3;
  checkers_game.board[3][3] = 2;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[3][3] = 4;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player2_Regular_SW_Success) {
  Checkers checkers_game;
  int move_to[2] = {2, 2};
  checkers_game.active_player = 2;
  checkers_game.board[3][1] = 1;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[3][1] = 3;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player2_Regular_SE_Success) {
  Checkers checkers_game;
  int move_to[2] = {2, 2};
  checkers_game.active_player = 2;
  checkers_game.board[3][3] = 1;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[3][3] = 3;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player2_King_SW_Success) {
  Checkers checkers_game;
  int move_to[2] = {2, 2};
  checkers_game.active_player = 2;
  checkers_game.board[2][2] = 4;
  checkers_game.board[3][1] = 1;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[3][1] = 3;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player2_King_SE_Success) {
  Checkers checkers_game;
  int move_to[2] = {2, 2};
  checkers_game.active_player = 2;
  checkers_game.board[2][2] = 4;
  checkers_game.board[3][3] = 1;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[3][3] = 3;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player2_King_NW_Success) {
  Checkers checkers_game;
  int move_to[2] = {5, 3};
  checkers_game.active_player = 2;
  checkers_game.board[5][3] = 4;
  checkers_game.board[4][2] = 1;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[4][2] = 3;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Player2_King_NE_Success) {
  Checkers checkers_game;
  int move_to[2] = {5, 3};
  checkers_game.active_player = 2;
  checkers_game.board[5][3] = 4;
  checkers_game.board[4][4] = 1;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);

  checkers_game.board[4][4] = 3;
  assertEqual(checkers_game.Checkers_TurnOver(move_to), false);
}

test(Checkers_TurnOver_Regular_Success) {
  Checkers checkers_game;
  int move_to[2] = {4, 2};
  assertEqual(checkers_game.Checkers_TurnOver(move_to), true);
}

/**
 * Checkers_CanJump tests
 **/
test(Checkers_CanJump_Player1_Regular_NW_Success) {
  Checkers checkers_game;
  checkers_game.board[4][6] = 2;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[4][6] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player1_Regular_NE_Success) {
  Checkers checkers_game;
  checkers_game.board[4][2] = 2;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[4][2] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player1_King_NW_Success) {
  Checkers checkers_game;
  checkers_game.board[5][7] = 3;
  checkers_game.board[4][6] = 2;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[4][6] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player1_King_NE_Success) {
  Checkers checkers_game;
  checkers_game.board[5][1] = 3;
  checkers_game.board[4][2] = 2;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[4][2] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player1_King_SW_Success) {
  Checkers checkers_game;
  checkers_game.board[1][7] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[2][6] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player1_King_SE_Success) {
  Checkers checkers_game;
  checkers_game.board[1][1] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[2][2] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player2_Regular_SW_Success) {
  Checkers checkers_game;
  checkers_game.active_player = 2;
  checkers_game.board[3][5] = 1;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[3][5] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player2_Regular_SE_Success) {
  Checkers checkers_game;
  checkers_game.active_player = 2;
  checkers_game.board[3][1] = 1;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[3][1] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player2_King_SW_Success) {
  Checkers checkers_game;
  checkers_game.active_player = 2;
  checkers_game.board[2][6] = 4;
  checkers_game.board[3][5] = 1;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[3][5] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player2_King_SE_Success) {
  Checkers checkers_game;
  checkers_game.active_player = 2;
  checkers_game.board[2][0] = 4;
  checkers_game.board[3][1] = 1;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[3][1] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player2_King_NW_Success) {
  Checkers checkers_game;
  checkers_game.active_player = 2;
  checkers_game.board[6][6] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[5][5] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Player2_King_NE_Success) {
  Checkers checkers_game;
  checkers_game.active_player = 2;
  checkers_game.board[6][0] = 4;
  assertEqual(checkers_game.Checkers_CanJump(), true);

  checkers_game.board[5][1] = 3;
  assertEqual(checkers_game.Checkers_CanJump(), true);
}

test(Checkers_CanJump_Regular_Success) {
  Checkers checkers_game;
  assertEqual(checkers_game.Checkers_CanJump(), false);
}

/**
 * Checkers_Turn tests
 **/
test(Checkers_Turn_JumpLock_Fail) {
  Checkers checkers_game;
  checkers_game.jump_lock[2] = 1;
  checkers_game.jump_lock[0] = 0;
  checkers_game.jump_lock[1] = 0;

  int from[2] = {5, 1};
  int to[2] = {4, 0};

  assertEqual(checkers_game.Checkers_Turn(from, to), 0);
}

test(Checkers_Turn_OutOfBounds_Fail) {
  Checkers checkers_game;
  
  int from[2] = {8, 8};
  int to[2] = {10, 10};

  assertEqual(checkers_game.Checkers_Turn(from, to), 0);
}

test(Checkers_Turn_InvalidSquare_Fail) {
  Checkers checkers_game;

  int from[2] = {5, 2};
  int to[2] = {4, 3};

  assertEqual(checkers_game.Checkers_Turn(from, to), 0);
}

test(Checkers_Turn_InvalidMove_Fail) {
  Checkers checkers_game;

  int from[2] = {2, 2};
  int to[2] = {1, 1};

  assertEqual(checkers_game.Checkers_Turn(from, to), 0);
}

test(Checkers_Turn_IllegalMove_Fail) {
  Checkers checkers_game;

  int from[2] = {5, 3};
  int to[2] = {2, 2};

  assertEqual(checkers_game.Checkers_Turn(from, to), 0);
}

test(Checkers_Turn_NormalMove_NoKinging_Success) {
  Checkers checkers_game;

  int from[2] = {5, 3};
  int to[2] = {4, 4};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 2);
  assertEqual(checkers_game.Checkers_GetBoardAt(5, 3), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(4, 4), 1);
}

test(Checkers_Turn_NormalMove_Kinging_Success) {
  Checkers checkers_game;

  checkers_game.board[1][1] = 1;
  checkers_game.board[0][0] = 0; 

  int from[2] = {1, 1};
  int to[2] = {0, 0};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 2);
  assertEqual(checkers_game.Checkers_GetBoardAt(1, 1), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(0, 0), 3);
}

test(Checkers_Turn_NormalJump_Player1_Success) {
  Checkers checkers_game;

  checkers_game.board[4][2] = 2;

  int from[2] = {5, 3};
  int to[2] = {3, 1};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 2);
  assertEqual(checkers_game.Checkers_GetBoardAt(5, 3), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(3, 1), 1);
  assertEqual(checkers_game.Checkers_GetBoardAt(4, 2), 0);
  assertEqual(checkers_game.Checkers_GetP2Count(), 11);
}

test(Checkers_Turn_NormalJump_King1_Success) {
  Checkers checkers_game;

  checkers_game.board[5][3] = 3;
  checkers_game.board[4][2] = 2;

  int from[2] = {5, 3};
  int to[2] = {3, 1};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 2);
  assertEqual(checkers_game.Checkers_GetBoardAt(5, 3), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(3, 1), 3);
  assertEqual(checkers_game.Checkers_GetBoardAt(4, 2), 0);
  assertEqual(checkers_game.Checkers_GetP2Count(), 11);
}

test(Checkers_Turn_NormalJump_Player2_Success) {
  Checkers checkers_game;

  checkers_game.board[3][1] = 1;
  checkers_game.active_player = 2;

  int from[2] = {2, 2};
  int to[2] = {4, 0};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 1);
  assertEqual(checkers_game.Checkers_GetBoardAt(2, 2), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(4, 0), 2);
  assertEqual(checkers_game.Checkers_GetBoardAt(3, 1), 0);
  assertEqual(checkers_game.Checkers_GetP1Count(), 11);
}

test(Checkers_Turn_NormalJump_Player1Win_Success) {
  Checkers checkers_game;

  checkers_game.board[4][2] = 2;
  checkers_game.p2_count = 1;

  int from[2] = {5, 3};
  int to[2] = {3, 1};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 1);
  assertEqual(checkers_game.Checkers_GetBoardAt(5, 3), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(3, 1), 1);
  assertEqual(checkers_game.Checkers_GetBoardAt(4, 2), 0);
  assertEqual(checkers_game.Checkers_GetP2Count(), 0);
  assertEqual(checkers_game.Checkers_GetWin(), 1);
}

test(Checkers_Turn_NormalJump_Player1JumpLock_Success) {
  Checkers checkers_game;

  checkers_game.board[4][2] = 2;
  checkers_game.board[1][3] = 0;

  int from[2] = {5, 3};
  int to[2] = {3, 1};

  assertEqual(checkers_game.Checkers_Turn(from, to), 1);
  assertEqual(checkers_game.Checkers_GetActivePlayer(), 1);
  assertEqual(checkers_game.Checkers_GetBoardAt(5, 3), 0);
  assertEqual(checkers_game.Checkers_GetBoardAt(3, 1), 1);
  assertEqual(checkers_game.Checkers_GetBoardAt(4, 2), 0);
  assertEqual(checkers_game.Checkers_GetP2Count(), 11);
}

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
