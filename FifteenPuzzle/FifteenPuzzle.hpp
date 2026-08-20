#ifndef FIFTEEN_PUZZLE_H
#define FIFTEEN_PUZZLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class FifteenPuzzle {
private:
    // Game State
    int board[4][4];
    std::pair<int, int> emptySquare;
    bool gameOver;
    
    // Game Logic
    void makeMove(std::string move);
    bool isValidMove(std::string move);
    void displayBoard();
    void setUpBoard();
    
public:
    FifteenPuzzle();
    
    // Game Control
    void runFifteenPuzzleGame();
    bool isGameOver();
    
    // Rendering The Game
    bool draw(sf::RenderWindow& window);
};

#endif
