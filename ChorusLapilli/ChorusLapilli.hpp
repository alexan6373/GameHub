#ifndef CHORUSLAPILLI_H
#define CHORUSLAPILLI_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class ChorusLapilli {
private:
    // Game State
    char playerToMove;
    char board[3][3];
    bool gameOver;
    
    // Game Logic
    void updateBoard(int move);
    bool isWinner(char player);
    bool isBoardFull();
    
public:
    ChorusLapilli();
    
    // Game Control
    bool isGameOver();
    void runTicTacToe();
    
    // Rendering The Game
    bool draw(sf::RenderWindow& window);
};

#endif
