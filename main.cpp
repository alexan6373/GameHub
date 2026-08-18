#include <iostream>
#include "FifteenPuzzle/FifteenPuzzle.hpp"
#include "PenguinPalooza/PenguinPalooza.hpp"
#include "Hangman/Hangman.hpp"
#include "TicTacToe/TicTacToe.hpp"

int main() {
    std::cout << R"(-----------------------------------------------------------------------------

 _______ _   _ ______    _____          __  __ ______    _    _ _    _ ____  
|__   __| | | |  ____|  / ____|   /\   |  \/  |  ____|  | |  | | |  | |  _ \ 
   | |  | |_| | |__    | |  __   /  \  | \  / | |__     | |__| | |  | | |_) |
   | |  |  _  |  __|   | | |_ | / /\ \ | |\/| |  __|    |  __  | |  | |  _ < 
   | |  | | | | |____  | |__| |/ ____ \| |  | | |____   | |  | | |__| | |_) |
   |_|  |_| |_|______|  \_____/_/    \_\_|  |_|______|  |_|  |_|\____/|____/

-----------------------------------------------------------------------------
    )";
   
    std::cout << "\nWelcome to the GameHub, with many classic games from 15 Puzzle to Hangman to Tic Tac Toe.\n";
    int choice = 0;
    
    while (true) {
        std::cout << R"(
==================================================
                 SELECT A GAME
==================================================

  Single Player
  -------------
    [1] 15 Puzzle
    [2] Hangman
    [3] Penguin Palooza

  Two Player
  ----------
    [4] Tic Tac Toe
    [5] Chorus Lapilli

  Other
  ----------
    [0] Quit

==================================================

Enter your choice: )";
        
        std::cin >> choice;
        std::cin.ignore(1000, '\n');
        std::cout << std::endl;
        
        if (choice == 0) {
            std::cout << "Thanks for playing!!!!!!\n";
            break;
        } else if (choice == 1) {
            std::cout << "=============================\n";
            std::cout << "Starting a game of 15 Puzzle.\n";
            std::cout << "=============================\n";
            FifteenPuzzle fp;
            fp.runFifteenPuzzleGame();
            
        } else if (choice == 2) {
            std::cout << "===========================\n";
            std::cout << "Starting a game of Hangman.\n";
            std::cout << "===========================\n";
            Hangman hangman;
            hangman.runHangmanGame();
            
        } else if (choice == 3) {
            std::cout << "===================================\n";
            std::cout << "Starting a game of Penguin Palooza.\n";
            std::cout << "===================================\n";
            runPenguinGame();
            
        } else if (choice == 4) {
            std::cout << "=============================\n";
            std::cout << "Starting a game of TicTacToe.\n";
            std::cout << "=============================\n";
            TicTacToe tictactoe;
            tictactoe.runTicTacToe();
        } else {
            std::cout << "The feature is either not available or currently being developed\n";
        }
    }
    
    return 0;
}
