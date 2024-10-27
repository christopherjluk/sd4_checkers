class Checkers
{
    public:
        Checkers();
        int getBoardAt(int, int);
        int get_p1_count();
        int get_p2_count();
        int getActivePlayer();
        int getWin();
        bool turnOver(int[2]);
        bool canJump();
        int turn(int[2], int[2]);
    private:
        int board[8][8];
        int p1_count;
        int p2_count;
        int activePlayer;
        int jumpLock[3];
        bool won;
};

Checkers::Checkers()
{
    p1_count = 12;
    p2_count = 12;
    activePlayer = 1;
    jumpLock[2] = 0;
    won = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i > 4 && ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)))
            {
                board[i][j] = 1;
            }
            else if (i < 3 && ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)))
            {
                board[i][j] = 2;
            }
            else
            {
                board[i][j] = 0;
            }
        }
    }
}

int Checkers::getBoardAt(int row, int col)
{
    return board[row][col];
}

int Checkers::get_p1_count()
{
    return p1_count;
}

int Checkers::get_p2_count()
{
    return p2_count;
}

int Checkers::getActivePlayer()
{
    return activePlayer;
}

int Checkers::getWin()
{
    return won;
}

bool Checkers::turnOver(int to[2])
{
    int i = to[0];
    int j = to[1];
    if (board[i][j] == 1 && activePlayer == 1)
    {
        if (i > 1 && j > 1 && board[i - 2][j - 2] == 0 && (board[i - 1][j - 1] == 2 || board[i - 1][j - 1] == 4))
        {
            return false;
        }
        if (i > 1 && j < 6 && board[i - 2][j + 2] == 0 && (board[i - 1][j + 1] == 2 || board[i - 1][j + 1] == 4))
        {
            return false;
        }
    }
    else if (board[i][j] == 3 && activePlayer == 1)
    {
        if (i > 1 && j > 1 && board[i - 2][j - 2] == 0 && (board[i - 1][j - 1] == 2 || board[i - 1][j - 1] == 4))
        {
            return false;
        }
        if (i > 1 && j < 6 && board[i - 2][j + 2] == 0 && (board[i - 1][j + 1] == 2 || board[i - 1][j + 1] == 4))
        {
            return false;
        }
        if (i < 6 && j > 1 && board[i + 2][j - 2] == 0 && (board[i + 1][j - 1] == 2 || board[i + 1][j - 1] == 4))
        {
            return false;
        }
        if (i < 6 && j < 6 && board[i + 2][j + 2] == 0 && (board[i + 1][j + 1] == 2 || board[i + 1][j + 1] == 4))
        {
            return false;
        }
    }
    if (board[i][j] == 2 && activePlayer == 2)
    {
        if (i < 6 && j > 1 && board[i + 2][j - 2] == 0 && (board[i + 1][j - 1] == 1 || board[i + 1][j - 1] == 3))
        {
            return false;
        }
        if (i < 6 && j < 6 && board[i + 2][j + 2] == 0 && (board[i + 1][j + 1] == 1 || board[i + 1][j + 1] == 3))
        {
            return false;
        }
    }
    else if (board[i][j] == 4 && activePlayer == 2)
    {
        if (i > 1 && j > 1 && board[i - 2][j - 2] == 0 && (board[i - 1][j - 1] == 1 || board[i - 1][j - 1] == 3))
        {
            return false;
        }
        if (i > 1 && j < 6 && board[i - 2][j + 2] == 0 && (board[i - 1][j + 1] == 1 || board[i - 1][j + 1] == 3))
        {
            return false;
        }
        if (i < 6 && j > 1 && board[i + 2][j - 2] == 0 && (board[i + 1][j - 1] == 1 || board[i + 1][j - 1] == 3))
        {
            return false;
        }
        if (i < 6 && j < 6 && board[i + 2][j + 2] == 0 && (board[i + 1][j + 1] == 1 || board[i + 1][j + 1] == 3))
        {
            return false;
        }
    }
    return true;
}

bool Checkers::canJump()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == 1 && activePlayer == 1)
            {
                if (i > 1 && j > 1 && board[i - 2][j - 2] == 0 && (board[i - 1][j - 1] == 2 || board[i - 1][j - 1] == 4))
                {
                    return true;
                }
                if (i > 1 && j < 6 && board[i - 2][j + 2] == 0 && (board[i - 1][j + 1] == 2 || board[i - 1][j + 1] == 4))
                {
                    return true;
                }
            }
            else if (board[i][j] == 3 && activePlayer == 1)
            {
                if (i > 1 && j > 1 && board[i - 2][j - 2] == 0 && (board[i - 1][j - 1] == 2 || board[i - 1][j - 1] == 4))
                {
                    return true;
                }
                if (i > 1 && j < 6 && board[i - 2][j + 2] == 0 && (board[i - 1][j + 1] == 2 || board[i - 1][j + 1] == 4))
                {
                    return true;
                }
                if (i < 6 && j > 1 && board[i + 2][j - 2] == 0 && (board[i + 1][j - 1] == 2 || board[i + 1][j - 1] == 4))
                {
                    return true;
                }
                if (i < 6 && j < 6 && board[i + 2][j + 2] == 0 && (board[i + 1][j + 1] == 2 || board[i + 1][j + 1] == 4))
                {
                    return true;
                }
            }
            if (board[i][j] == 2 && activePlayer == 2)
            {
                if (i < 6 && j > 1 && board[i + 2][j - 2] == 0 && (board[i + 1][j - 1] == 1 || board[i + 1][j - 1] == 3))
                {
                    return true;
                }
                if (i < 6 && j < 6 && board[i + 2][j + 2] == 0 && (board[i + 1][j + 1] == 1 || board[i + 1][j + 1] == 3))
                {
                    return true;
                }
            }
            else if (board[i][j] == 4 && activePlayer == 2)
            {
                if (i > 1 && j > 1 && board[i - 2][j - 2] == 0 && (board[i - 1][j - 1] == 1 || board[i - 1][j - 1] == 3))
                {
                    return true;
                }
                if (i > 1 && j < 6 && board[i - 2][j + 2] == 0 && (board[i - 1][j + 1] == 1 || board[i - 1][j + 1] == 3))
                {
                    return true;
                }
                if (i < 6 && j > 1 && board[i + 2][j - 2] == 0 && (board[i + 1][j - 1] == 1 || board[i + 1][j - 1] == 3))
                {
                    return true;
                }
                if (i < 6 && j < 6 && board[i + 2][j + 2] == 0 && (board[i + 1][j + 1] == 1 || board[i + 1][j + 1] == 3))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int Checkers::turn(int from[2], int to[2])
{
    if (jumpLock[2] == 1 && (from[0] != jumpLock[0] || from[1] != jumpLock[1]))
    {
        return 0;
    }
    if (from[0] < 0 || from[0] >= 8 || from[1] < 0 || from[1] >= 8 || to[0] < 0 || to[0] >= 8 || to[1] < 0 || to[1] >= 8)
    {
        return 0;
    }
    if ((!(from[0] % 2 == 0 && from[1] % 2 == 0) && !(from[0] % 2 == 1 && from[1] % 2 == 1)) || (!(to[0] % 2 == 0 && to[1] % 2 == 0) && !(to[0] % 2 == 1 && to[1] % 2 == 1)))
    {
        return 0;
    }
    if (board[from[0]][from[1]] != activePlayer && board[from[0]][from[1]] != activePlayer + 2)
    {
        return 0;
    }
    if (!canJump() && board[to[0]][to[1]] == 0 && ((board[from[0]][from[1]] == 1 && (to[0] == from[0] - 1 && (to[1] == from[1] - 1 || to[1] == from[1] + 1))) || (board[from[0]][from[1]] == 2 && (to[0] == from[0] + 1 && (to[1] == from[1] - 1 || to[1] == from[1] + 1))) || ((board[from[0]][from[1]] == 3 || board[from[0]][from[1]] == 4) && ((to[0] == from[0] - 1 || to[0] == from[0] + 1) && (to[1] == from[1] - 1 || to[1] == from[1] + 1)))))
    {
        if ((board[from[0]][from[1]] == 1 && to[0] == 0) || (board[from[0]][from[1]] == 2 && to[0] == 7))
        {
            board[to[0]][to[1]] = board[from[0]][from[1]] + 2;
        }
        else
        {
            board[to[0]][to[1]] = board[from[0]][from[1]];
        }
        board[from[0]][from[1]] = 0;
    }
    else if (board[to[0]][to[1]] == 0 && ((board[from[0]][from[1]] == 1 && (to[0] == from[0] - 2 && ((to[1] == from[1] - 2 && (board[from[0]-1][from[1]-1] == 2 || board[from[0]-1][from[1]-1] == 4)) || (to[1] == from[1] + 2 && (board[from[0]-1][from[1]+1] == 2 || board[from[0]-1][from[1]+1] == 4))))) || (board[from[0]][from[1]] == 2 && (to[0] == from[0] + 2 && ((to[1] == from[1] - 2 && (board[from[0]+1][from[1]-1] == 1 || board[from[0]+1][from[1]-1] == 3)) || (to[1] == from[1] + 2 && (board[from[0]+1][from[1]+1] == 1 || board[from[0]+1][from[1]+1] == 3)))))))
    {
        if ((board[from[0]][from[1]] == 1 && to[0] == 0) || (board[from[0]][from[1]] == 2 && to[0] == 7))
        {
            board[to[0]][to[1]] = board[from[0]][from[1]] + 2;
        }
        else
        {
            board[to[0]][to[1]] = board[from[0]][from[1]];
        }
        board[from[0]][from[1]] = 0;
        board[(to[0]+from[0])/2][(to[1]+from[1])/2] = 0;

        if (activePlayer == 1)
        {
            p2_count = p2_count - 1;
        }
        else if (activePlayer == 2)
        {
            p1_count = p1_count - 1;
        }
        if (p1_count == 0 || p2_count == 0)
        {
            won = 1;
            return 1;
        }
        if(!turnOver(to))
        {
            jumpLock[0] = to[0];
            jumpLock[1] = to[1];
            jumpLock[2] = 1;
            return 1;
        }
    }
    else if (board[to[0]][to[1]] == 0 && (to[0] == from[0] - 2 || to[0] == from[0] + 2) && (to[1] == from[1] - 2 || to[1] == from[1] + 2) && (board[from[0]][from[1]] == 3 && (board[(to[0]+from[0])/2][(to[1]+from[1])/2] == 2 || board[(to[0]+from[0])/2][(to[1]+from[1])/2] == 4)) || (board[from[0]][from[1]] == 4 && (board[(to[0]+from[0])/2][(to[1]+from[1])/2] == 1 || board[(to[0]+from[0])/2][(to[1]+from[1])/2] == 3)))
    {
        board[to[0]][to[1]] = board[from[0]][from[1]];
        board[from[0]][from[1]] = 0;
        board[(to[0]+from[0])/2][(to[1]+from[1])/2] = 0;

        if (activePlayer == 1)
        {
            p2_count = p2_count - 1;
        }
        else if (activePlayer == 2)
        {
            p1_count = p1_count - 1;
        }
        if (p1_count == 0 || p2_count == 0)
        {
            won = 1;
            return 1;
        }
        if(!turnOver(to))
        {
            jumpLock[0] = to[0];
            jumpLock[1] = to[1];
            jumpLock[2] = 1;
            return 1;
        }
    }
    else
    {
        return 0;
    }

    jumpLock[2] = 0;
    activePlayer = 3 - activePlayer;
    return 1;
}