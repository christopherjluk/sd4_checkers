/************************************************************
 * @file Checkers.cpp
 * @brief The implementation for the Checkers game algorithm
 ************************************************************/

/**********************************
 ** Library Includes
 **********************************/
#include "Checkers.h"

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
 ** Function Definitions
 **********************************/
/**
 * The constructor for a Checkers object, initializes all of the members
 *
 */
Checkers::Checkers() {
  /* Initializes the members */
  p1_count = 12;
  p2_count = 12;
  active_player = 1;
  jump_lock[2] = 0;
  won = 0;

  /* Initializes the game board */
  for (int i = 0; i < 8; i++) {   /* For iterating through the rows */
    for (int j = 0; j < 8; j++) { /* For iterating through the columns */
      /* Initializes player 1's pieces */
      if (i > 4 && ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1))) {
        board[i][j] = 1;
      }
      /* Initializes player 2's pieces */
      else if (i < 3 && ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1))) {
        board[i][j] = 2;
      }
      /* Initializes empty squares */
      else {
        board[i][j] = 0;
      }
    }
  }
}

/**
 * Retrieve the state of a square based on the row and column
 *
 * @param row: The row of the board to retrieve
 * @param col: The column of the board to retrieve
 * @return int: The state of the specified square
 */
int Checkers::Checkers_GetBoardAt(int row, int col) {
  return board[row][col];
}

/**
 * Retrieve how many pieces player 1 currently has
 *
 * @return int: The number of pieces player 1 has
 */
int Checkers::Checkers_GetP1Count() {
  return p1_count;
}

/**
 * Retrieve how many pieces player 2 currently has
 *
 * @return int: The number of pieces player 2 has
 */
int Checkers::Checkers_GetP2Count() {
  return p2_count;
}

/**
 * Retrieves the active turn of the player
 *
 * @return int: The turn of the corresponding player
 */
int Checkers::Checkers_GetActivePlayer() {
  return active_player;
}

/**
 * Retrieves if any player has won
 *
 * @return int: If any player has won (0=No, 1=Yes)
 */
int Checkers::Checkers_GetWin() {
  return won;
}

/**
 * Checks if there is still required moves left in a turn for a player
 *
 * @return bool: If there is still a move left for the active player
 */
bool Checkers::Checkers_TurnOver(int to[2]) {
  int row = to[0];
  int col = to[1];
  /* For player 1's regular pieces */
  if (board[row][col] == 1 && active_player == 1) {
    /* Checks if player 1's regular piece has a jump available moving up the board to the left */
    if (row > 1 && col > 1 && board[row - 2][col - 2] == 0 && (board[row - 1][col - 1] == 2 || board[row - 1][col - 1] == 4)) {
      return false;
    }
    
    /* Checks if player 1's regular piece has a jump available moving up the board to the right */
    if (row > 1 && col < 6 && board[row - 2][col + 2] == 0 && (board[row - 1][col + 1] == 2 || board[row - 1][col + 1] == 4)) {
      return false;
    }
  }
  /* For player's 1 king pieces */
  else if (board[row][col] == 3 && active_player == 1) {
    /* Checks if player 1's king piece has a jump available moving up the board to the left */
    if (row > 1 && col > 1 && board[row - 2][col - 2] == 0 && (board[row - 1][col - 1] == 2 || board[row - 1][col - 1] == 4)) {
      return false;
    }

    /* Checks if player 1's king piece has a jump available moving up the board to the right */
    if (row > 1 && col < 6 && board[row - 2][col + 2] == 0 && (board[row - 1][col + 1] == 2 || board[row - 1][col + 1] == 4)) {
      return false;
    }

    /* Checks if player 1's king piece has a jump available moving down the board to the left */
    if (row < 6 && col > 1 && board[row + 2][col - 2] == 0 && (board[row + 1][col - 1] == 2 || board[row + 1][col - 1] == 4)) {
      return false;
    }

    /* Checks if player 1's king piece has a jump available moving down the board to the right */
    if (row < 6 && col < 6 && board[row + 2][col + 2] == 0 && (board[row + 1][col + 1] == 2 || board[row + 1][col + 1] == 4)) {
      return false;
    }
  }
  /* For player 2's regular pieces */
  else if (board[row][col] == 2 && active_player == 2) {
    /* Checks if player 2's regular piece has a jump available moving down the board to the left */
    if (row < 6 && col > 1 && board[row + 2][col - 2] == 0 && (board[row + 1][col - 1] == 1 || board[row + 1][col - 1] == 3)) {
      return false;
    }
    
    /* Checks if player 2's regular piece has a jump available moving down the board to the right */
    if (row < 6 && col < 6 && board[row + 2][col + 2] == 0 && (board[row + 1][col + 1] == 1 || board[row + 1][col + 1] == 3)) {
      return false;
    }
  }
  /* For player 2's king pieces */
  else if (board[row][col] == 4 && active_player == 2) {
    /* Checks if player 2's king piece has a jump available moving up the board to the left */
    if (row > 1 && col > 1 && board[row - 2][col - 2] == 0 && (board[row - 1][col - 1] == 1 || board[row - 1][col - 1] == 3)) {
      return false;
    }

    /* Checks if player 2's king piece has a jump available moving up the board to the right */
    if (row > 1 && col < 6 && board[row - 2][col + 2] == 0 && (board[row - 1][col + 1] == 1 || board[row - 1][col + 1] == 3)) {
      return false;
    }

    /* Checks if player 2's king piece has a jump available moving down the board to the left */
    if (row < 6 && col > 1 && board[row + 2][col - 2] == 0 && (board[row + 1][col - 1] == 1 || board[row + 1][col - 1] == 3)) {
      return false;
    }
    
    /* Checks if player 2's king piece has a jump available moving down the board to the right */
    if (row < 6 && col < 6 && board[row + 2][col + 2] == 0 && (board[row + 1][col + 1] == 1 || board[row + 1][col + 1] == 3)) {
      return false;
    }
  }

  /* If none of these conditions meet, then return that the turn is over */
  return true;
}

/**
 * Checks if there is a jump available for the active player
 *
 * @return bool: If there is a jump available for a player
 */
bool Checkers::Checkers_CanJump() {
  /* Iterates through each row on the checkerboard */
  for (int row = 0; row < 8; row++) {
    /* Iterates through each column on the checkerboard */
    for (int col = 0; col < 8; col++) {
      /* For player 1's regular pieces */
      if (board[row][col] == 1 && active_player == 1) {
        /* Checks if player 1's regular piece has a jump available moving up the board to the left */
        if (row > 1 && col > 1 && board[row - 2][col - 2] == 0 && (board[row - 1][col - 1] == 2 || board[row - 1][col - 1] == 4)) {
          return true;
        }

        /* Checks if player 1's regular piece has a jump available moving up the board to the right */
        if (row > 1 && col < 6 && board[row - 2][col + 2] == 0 && (board[row - 1][col + 1] == 2 || board[row - 1][col + 1] == 4)) {
          return true;
        }
      }
      /* For player 1's king pieces */
      if (board[row][col] == 3 && active_player == 1) {
        /* Checks if player 1's king piece has a jump available moving up the board to the left */
        if (row > 1 && col > 1 && board[row - 2][col - 2] == 0 && (board[row - 1][col - 1] == 2 || board[row - 1][col - 1] == 4)) {
          return true;
        }

        /* Checks if player 1's king piece has a jump available moving up the board to the right */
        if (row > 1 && col < 6 && board[row - 2][col + 2] == 0 && (board[row - 1][col + 1] == 2 || board[row - 1][col + 1] == 4)) {
          return true;
        }

        /* Checks if player 1's king piece has a jump available moving down the board to the left */
        if (row < 6 && col > 1 && board[row + 2][col - 2] == 0 && (board[row + 1][col - 1] == 2 || board[row + 1][col - 1] == 4)) {
          return true;
        }

        /* Checks if player 1's king piece has a jump available moving down the board to the right */
        if (row < 6 && col < 6 && board[row + 2][col + 2] == 0 && (board[row + 1][col + 1] == 2 || board[row + 1][col + 1] == 4)) {
          return true;
        }
      }
      /* For player 2's regular pieces */
      if (board[row][col] == 2 && active_player == 2) {
        /* Checks if player 2's regular piece has a jump available moving down the board to the left */
        if (row < 6 && col > 1 && board[row + 2][col - 2] == 0 && (board[row + 1][col - 1] == 1 || board[row + 1][col - 1] == 3)) {
          return true;
        }
        
        /* Checks if player 2's regular piece has a jump available moving down the board to the right */
        if (row < 6 && col < 6 && board[row + 2][col + 2] == 0 && (board[row + 1][col + 1] == 1 || board[row + 1][col + 1] == 3)) {
          return true;
        }
      }
      /* For player 2's king pieces */
      if (board[row][col] == 4 && active_player == 2) {
        /* Checks if player 2's king piece has a jump available moving up the board to the left */
        if (row > 1 && col > 1 && board[row - 2][col - 2] == 0 && (board[row - 1][col - 1] == 1 || board[row - 1][col - 1] == 3)) {
          return true;
        }

        /* Checks if player 2's king piece has a jump available moving up the board to the right */
        if (row > 1 && col < 6 && board[row - 2][col + 2] == 0 && (board[row - 1][col + 1] == 1 || board[row - 1][col + 1] == 3)) {
          return true;
        }

        /* Checks if player 2's king piece has a jump available moving down the board to the left */
        if (row < 6 && col > 1 && board[row + 2][col - 2] == 0 && (board[row + 1][col - 1] == 1 || board[row + 1][col - 1] == 3)) {
          return true;
        }
        
        /* Checks if player 2's king piece has a jump available moving down the board to the right */
        if (row < 6 && col < 6 && board[row + 2][col + 2] == 0 && (board[row + 1][col + 1] == 1 || board[row + 1][col + 1] == 3)) {
          return true;
        }
      }
    }
  }

  /* If none of these conditions meet, then return that there are no jumps */
  return false;
}

/**
 * A turn (or a partial turn) for a player, where a piece will move from one spot to another
 *
 * @param from: The square where the desired piece to move is
 * @param to:   The square to move the desired piece to
 */
int Checkers::Checkers_Turn(int from[2], int to[2]) {
  /* If the jump lock indicates a jump but doesn't match the square, return that move was invalid */
  if (jump_lock[2] == 1 && (from[0] != jump_lock[0] || from[1] != jump_lock[1])) {
    return 0;
  }

  /* If any of the desired squares are out of bounds, return that move was invalid */
  if (from[0] < 0 || from[0] >= 8 || from[1] < 0 || from[0] >= 8 || to[0] < 0 || to[0] >= 8 || to[1] < 0 || to[1] >= 8) {
    return 0;
  }

  /* If a move is to an invalid square, return that move was invalid */
  if ((!(from[0] % 2 == 0 && from[1] % 2 == 1) && !(from[0] % 2 == 1 && from[1] % 2 == 0)) || /* Checks if from is valid */
      (!(to[0] % 2 == 0 && to[1] % 2 == 1) && !(to[0] % 2 == 1 && to[1] % 2 == 0))) { /* Checks if to is valid */
    return 0;
  }

  /* If a player tries to move a piece from a square that does not have their piece, return that move was invalid */
  if (board[from[0]][from[1]] != active_player && board[from[0]][from[1]] != (active_player + 2)) {
    return 0;
  }

  /* If there is no jump available and an adjacent diagonal square is open (up for player 1, down for player 2, both for kings), then the move can be done */
  if (!Checkers_CanJump() && board[to[0]][to[1]] == 0 && /* Checks if there is a jump and if the desired space is empty */
      ((board[from[0]][from[1]] == 1 && (to[0] == from[0] - 1 && (to[1] == from[1] - 1 || to[1] == from[1] + 1))) || /* Checks if the space is adjacent diagonal upwards (piece 1) */
       (board[from[0]][from[1]] == 2 && (to[0] == from[0] + 1 && (to[1] == from[1] - 1 || to[1] == from[1] + 1))) || /* Checks if the space is adjacent diagonal downwards (piece 2) */ 
       ((board[from[0]][from[1]] == 3 || board[from[0]][from[1]] == 4) && ((to[0] == from[0] - 1 || to[0] == from[0] + 1) && (to[1] == from[1] - 1 || to[1] == from[1] + 1))))) { /* Checks if the space is adjacent diagonal (king) */
    /* Checks if the move results in a kinging */
    if ((board[from[0]][from[1]] == 1 && to[0] == 0) || (board[from[0]][from[1]] == 2 && to[0] == 7)) {
      board[to[0]][to[1]] = board[from[0]][from[1]] + 2;
    }
    /* Otherwise, update the new square with the piece */
    else {
      board[to[0]][to[1]] = board[from[0]][from[1]];
    }
    
    /* Clear the original square */
    board[from[0]][from[1]] = 0;
  }
  /* If there is a jump available for a regular piece (with the proper conditions met where an empty square follows an opposing piece), then the move can be valid */
  else if (board[to[0]][to[1]] == 0 && /* Checks if the desired space is empty */
           ((board[from[0]][from[1]] == 1 && (to[0] == from[0] - 2 && /* Checks if the space is upwards with the jump (piece 1) */
             ((to[1] == from[1] - 2 && (board[from[0] - 1][from[1] - 1] == 2 || board[from[0] - 1][from[1] - 1] == 4)) || /* Checks if there is an opposing piece in between to the left */ 
              (to[1] == from[1] + 2 && (board[from[0] - 1][from[1] + 1] == 2 || board[from[0] - 1][from[1] + 1] == 4))))) || /* Checks if there is an opposing piece in between to the right */
            (board[from[0]][from[1]] == 2 && (to[0] == from[0] + 2 && /* Checks if the space is downwards with the jump (piece 2) */
             ((to[1] == from[1] - 2 && (board[from[0] + 1][from[1] - 1] == 1 || board[from[0] + 1][from[1] - 1] == 3)) || /* Checks if there is an opposing piece in between to the left */ 
              (to[1] == from[1] + 2 && (board[from[0] + 1][from[1] + 1] == 1 || board[from[0] + 1][from[1] + 1] == 3))))))) { /* Checks if there is an opposing piece in between to the right */
    /* Checks if the move results in a kinging */
    if ((board[from[0]][from[1]] == 1 && to[0] == 0) || (board[from[0]][from[1]] == 2 && to[0] == 7)) {
      board[to[0]][to[1]] = board[from[0]][from[1]] + 2;
    }
    /* Otherwise, update the new square with the piece */
    else {
      board[to[0]][to[1]] = board[from[0]][from[1]];
    }

    /* Clear the original square */
    board[from[0]][from[1]] = 0;

    /* Remove the piece that was jumped */
    board[(to[0] + from[0]) / 2][(to[1] + from[1]) / 2] = 0;

    /* If player 1 has the active turn, remove one piece from player 2's count */
    if (active_player == 1) {
      p2_count = p2_count - 1;
    }
    /* If player 2 has the active turn, remove one piece from player 1's count */
    else if (active_player == 2) {
      p1_count = p1_count - 1;
    }

    /* If one player has no more pieces, then the game ends (with the winner variable being set and the active player being the winner) and return the move is valid */
    if (p1_count == 0 || p2_count == 0) {
      won = 1;
      return 1;
    }

    /* If there are still more jump conditions available, then that player's turn is not over and moves are locked for the jump (and variable is set) */
    if(!Checkers_TurnOver(to)) {
      jump_lock[0] = to[0];
      jump_lock[1] = to[1];
      jump_lock[2] = 1;
      return 1;
    }
  }
  /* If there is a jump available for a king piece (with the proper conditions met where an empty square follows an opposing piece), then the move can be valid */
  else if (board[to[0]][to[1]] == 0 && /* Checks if the desired space is empty */
           (to[0] == from[0] - 2 || to[0] == from[0] + 2) && (to[1] == from[1] - 2 || to[1] == from[1] + 2) && /* Checks if the space is a valid jump space */
           (board[from[0]][from[1]] == 3 && (board[(to[0] + from[0]) / 2][(to[1] + from[1]) / 2] == 2 || board[(to[0] + from[0]) / 2][(to[1] + from[1]) / 2] == 4)) || /* Checks if there is an opposing piece in between (player 1) */
           (board[from[0]][from[1]] == 4 && (board[(to[0] + from[0]) / 2][(to[1] + from[1]) / 2] == 1 || board[(to[0] + from[0]) / 2][(to[1] + from[1]) / 2] == 3))) { /* Checks if there is an opposing piece in between (player 2) */
    /* Update the new square with the current piece */
    board[to[0]][to[1]] = board[from[0]][from[1]];

    /* Clear the original square */
    board[from[0]][from[1]] = 0;
    
    /* Remove the piece that was jumped */
    board[(to[0] + from[0]) / 2][(to[1] + from[1]) / 2] = 0;

    /* If player 1 has the active turn, remove one piece from player 2's count */
    if (active_player == 1) {
      p2_count = p2_count - 1;
    }
    /* If player 2 has the active turn, remove one piece from player 1's count */
    else if (active_player == 2) {
      p1_count = p1_count - 1;
    }

    /* If one player has no more pieces, then the game ends (with the winner variable being set and the active player being the winner) and return the move is valid */
    if (p1_count == 0 || p2_count == 0) {
      won = 1;
      return 1;
    }

    /* If there are still more jump conditions available, then that player's turn is not over and moves are locked for the jump (and variable is set) */
    if(!Checkers_TurnOver(to)) {
      jump_lock[0] = to[0];
      jump_lock[1] = to[1];
      jump_lock[2] = 1;
      return 1;
    }
  }
  /* If none of these conditions meet, return an invalid move */
  else {
    return 0;
  }

  /* If the turn needs to change, ensure the jump lock is 0 and the active player changes before returning that the move was valid */
  jump_lock[2] = 0;
  active_player = 3 - active_player;
  return 1;
}
