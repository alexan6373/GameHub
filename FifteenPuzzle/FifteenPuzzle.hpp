#ifndef FIFTEEN_PUZZLE_H
#define FIFTEEN_PUZZLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class FifteenPuzzle {
private:
    int board[4][4];
    std::pair<int, int> emptySquare;
    bool gameOver;
    
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
    bool draw(sf::RenderWindow& window);
};

#endif
