#ifndef TICTACTOE_H
#define TICTACTOE_H

class TicTacToe {
private:
    char playerToMove;
    char board[3][3];
    
    void updateBoard(int move);
    void displayBoard();
    bool isWinner(char player);
    bool isBoardFull();
    
public:
    TicTacToe();
    void runTicTacToe();
};

#endif
