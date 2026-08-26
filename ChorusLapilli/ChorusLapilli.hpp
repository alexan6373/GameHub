#ifndef CHORUSLAPILLI_H
#define CHORUSLAPILLI_H

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class ChorusLapilli {
private:
    // Game State
    char playerToMove;
    char board[3][3];
    std::vector<std::vector<int>> legalMoves;
    bool gameOver;
    
    // Game Logic
    bool isWinner(char player);
    bool isValidMove(int start, int end);
    
public:
    ChorusLapilli();
    
    // Game Control
    void runTicTacToe();
    
    // Rendering The Game
    bool draw(sf::RenderWindow& window);
};

#endif
