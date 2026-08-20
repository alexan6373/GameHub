#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class TicTacToe {
private:
    // Game State
    char playerToMove;
    char board[3][3];
    bool gameOver;
    
    // Game Logic
    void updateBoard(int move);
    void displayBoard();
    bool isWinner(char player);
    bool isBoardFull();
    
public:
    TicTacToe();
    
    // Game Control
    bool isGameOver();
    void runTicTacToe();
    
    // Rendering
    bool draw(sf::RenderWindow& window);
};

#endif
