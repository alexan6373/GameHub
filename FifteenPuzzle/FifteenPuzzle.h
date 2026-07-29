#ifndef FIFTEEN_PUZZLE_H
#define FIFTEEN_PUZZLE_H

class FifteenPuzzle {
private:
    int board[4][4];
    std::pair<int, int> emptySquare;
    
    // Functions for making moves
    void makeMove(std::string move);
    bool isValidMove(std::string move);
    
    // Functions for managing the board
    bool isGameOver();
    void displayBoard();
    void setUpBoard();
    
public:
    // Constructor
    FifteenPuzzle();
    
    // Main game functions
    void runFifteenPuzzleGame();
};

#endif
