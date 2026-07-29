#include <iostream>
#include <random>
#include <string>
#include "FifteenPuzzle.h"

// Helper functions
void FifteenPuzzle::makeMove(std::string move) {
    int x = emptySquare.first;
    int y = emptySquare.second;
    
    if (move == "u") {
        board[x][y] = board[x + 1][y];
        board[x + 1][y] = 0;
        emptySquare.first++;
        
    } else if (move == "d") {
        board[x][y] = board[x - 1][y];
        board[x - 1][y] = 0;
        emptySquare.first--;
    } else if (move == "l") {
        board[x][y] = board[x][y + 1];
        board[x][y + 1] = 0;
        emptySquare.second++;
        
    } else if (move == "r") {
        board[x][y] = board[x][y - 1];
        board[x][y - 1] = 0;
        emptySquare.second--;
    }
}

bool FifteenPuzzle::isValidMove(std::string move) {
    if (move != "u" && move != "d" && move != "l" && move != "r")
        return false;
    else if (move == "u" && emptySquare.first == 3)
        return false;
    else if (move == "d" && emptySquare.first == 0)
        return false;
    else if (move == "l" && emptySquare.second == 3)
        return false;
    else if (move == "r" && emptySquare.second == 0l)
        return false;
    
    return true;
}

bool FifteenPuzzle::isGameOver() {
    int val = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((i != 3 || j != 3) && board[i][j] != val)
                return false;
            val++;
        }
    }
    
    return true;
}

void FifteenPuzzle::displayBoard() {
    std::cout << "---------------------\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << "| ";
            
            if (board[i][j] < 10)
                std::cout << " ";
            
            if (board[i][j] == 0)
                std::cout << "X ";
            else
                std::cout << board[i][j] << " ";
        }
        std::cout << "| " << std::endl;
    }
    std::cout << "---------------------\n";
}

void FifteenPuzzle::setUpBoard() {
    std::random_device rd;
    std::mt19937 mt(rd());
    
    // Shuffles numbers for insertion into board
    int numbers[15];
    for (int i = 0; i < 15; i++)
        numbers[i] = i + 1;
    
    emptySquare = {3, 3};
    
    int index = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i != emptySquare.first || j != emptySquare.second) {
                board[i][j] = numbers[index];
                index++;
            }
        }
    }
    
    for (int i = 0; i < 1000; i++) {
        int move = mt() % 4;
        std::string moves[4] = {"u", "d", "l", "r"};
        
        if (isValidMove(moves[move]))
            makeMove(moves[move]);
            
    }
}
    
// Constructor
FifteenPuzzle::FifteenPuzzle():
    board{{0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},
    emptySquare{0, 0} {
}

// Main game functions
void FifteenPuzzle::runFifteenPuzzleGame() {
    setUpBoard();
    
    while(true) {
        displayBoard();
        
        if (isGameOver()) {
            std::cout << "You have won. Congratulations." << std::endl;
            break;
        }
        
        std::string move = "";
        
        std::cout << "Moves\n";
        std::cout << "u: Up\n";
        std::cout << "d: Down\n";
        std::cout << "l: Left\n";
        std::cout << "r: Right\n";
        std::cout << "Enter q to quit the game.\n";
        std::cout << "Enter a move: ";
        std::getline(std::cin, move);
        
        if (move == "q") {
            break;
        } else if (!isValidMove(move)) {
            std::cout << "Invalid Move\n\n";
        } else {
            makeMove(move);
        }
    }
}
