/************************************************************
 * @file Checkers.h
 * @brief The header for the Checkers game algorithm
 * @note This file is copied over from src and modified for testing
 ************************************************************/
#ifndef CHECKERS_H
#define CHECKERS_H

/*********************************************
 ** Class Declarations and Function Prototypes
 *********************************************/
/* All functions are made public so tests can directly modify these values */
class Checkers {
  public:
    /* Originally Public Functions */
    Checkers();
    int  Checkers_GetBoardAt(int row, int col);
    int  Checkers_GetP1Count();
    int  Checkers_GetP2Count();
    int  Checkers_GetActivePlayer();
    int  Checkers_GetWin();
    int  Checkers_Turn(int from[2], int to[2]);

    /* Originally Private Functions */
    bool Checkers_HasMove();
    bool Checkers_TurnOver(int to[2]);
    bool Checkers_CanJump();

    /* Originally Private Members */
    int  board[8][8];     /* The active game map */
    int  p1_count;        /* The piece count for player 1 */
    int  p2_count;        /* The piece count for player 2 */
    int  active_player;   /* The active player's turn */
    int  jump_lock[3];    /* Indicator for if there is a jump available (First two indicies are the move and the third index indicates if there is a jump) */
    bool won;             /* Indicator for if there is a winner */
};

#endif /* CHECKERS_H */
