#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class TicTacToe {
private:
    char playerToMove;
    char board[3][3];
    bool gameOver;
    
//    Helper functions
    void updateBoard(int move);
    void displayBoard();
    bool isWinner(char player);
    bool isBoardFull();
    
public:
//    Constructor
    TicTacToe();
    void runTicTacToe();
    
//    Drawing functions
    bool draw(sf::RenderWindow& window);
    bool isGameOver();
};

#endif
