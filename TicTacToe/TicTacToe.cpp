//    playerToMove
//    char board[3][3]

#include "TicTacToe.hpp"
#include <iostream>

void TicTacToe::updateBoard(int move) {
    board[move / 3][move % 3] = playerToMove;
    playerToMove = playerToMove == 'X' ? 'O' : 'X';
}

void TicTacToe::displayBoard() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                std::cout << row * 3 + col << " ";
            } else {
                std::cout << board[row][col] << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool TicTacToe::isWinner(char player) {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] &&
            board[row][1] == board[row][2] &&
            board[row][0] == player) {
            return true;
        }
    }

    // Check columns
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] &&
            board[1][col] == board[2][col] &&
            board[0][col] != player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2] &&
        board[0][0] != player) {
        return true;
    }

    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0] &&
        board[0][2] != player) {
        return true;
    }
    
    return false;
}

bool TicTacToe::isBoardFull() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                return false;
            }
        }
    }
    
    return true;
}

TicTacToe::TicTacToe() : playerToMove('X'),
    board({' ', ' ', ' '},
          {' ', ' ', ' '},
          {' ', ' ', ' '}) {}

void TicTacToe::runTicTacToe() {
    int move;
    while (true) {
        displayBoard();
        
        std::cout << "Player " << playerToMove << ". Make a move: " << std::endl;
        std::cin >> move;
        std::cin.ignore(1000, '\n');
        
        if (move < 1 || move > 9 || board[move / 3][move % 3] != ' ') {
            std::cout << "Invalid move." << std::endl;
            continue;
        }
        
        updateBoard(move);
        
        if (isWinner(playerToMove)) {
            std::cout << "The game is over. Player " << playerToMove << " won." << std::endl;
        }
    }
}
