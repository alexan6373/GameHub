#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "FifteenPuzzle/FifteenPuzzle.hpp"
#include "PenguinPalooza/PenguinPalooza.hpp"
#include "Hangman/Hangman.hpp"
#include "TicTacToe/TicTacToe.hpp"
#include "GraphicsHelper.hpp"

int main() {
    // Window creation
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "GameHub", sf::Style::Default);
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    
    enum class Screen {
        MainMenu,
        TicTacToe,
    };
    Screen currentScreen = Screen::MainMenu;
    
    // Text and buttons
    sf::Text title = createText(font, "The Game Hub", 80.f, window.getSize().x / 2.f, 50.f);
    
    sf::RectangleShape ticTacToeButton = createButton({300.f, 80.f}, {window.getSize().x / 2.f - 150.f, 300.f}, 3.f);
    sf::Text ticTacToeText = createText(font, "Tic Tac Toe", 30.f, window.getSize().x / 2.f, 340.f);

    // Game objects
    TicTacToe ticTacToe;
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePosition(
                        static_cast<float>(mouseButtonPressed->position.x),
                        static_cast<float>(mouseButtonPressed->position.y)
                    );
                    
                    if (ticTacToeButton.getGlobalBounds().contains(mousePosition)) {
                        currentScreen = Screen::TicTacToe;
                    }
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140));
        
        if (currentScreen == Screen::MainMenu) {
            window.draw(title);
            window.draw(ticTacToeButton);
            window.draw(ticTacToeText);
        } else {
            if (ticTacToe.draw(window))
                currentScreen = Screen::MainMenu;
        }
        
        window.display();
    }
    return 0;
}
    
//    std::cout << "\nWelcome to the GameHub, with many classic games from 15 Puzzle to Hangman to Tic Tac Toe.\n";
//    int choice = 0;
    
//    while (true) {
//        std::cout << R"(
//==================================================
//                 SELECT A GAME
//==================================================
//
//  Single Player
//  -------------
//    [1] 15 Puzzle
//    [2] Hangman
//    [3] Penguin Palooza
//
//  Two Player
//  ----------
//    [4] Tic Tac Toe
//    [5] Chorus Lapilli
//
//  Other
//  ----------
//    [0] Quit
//
//==================================================
//
//Enter your choice: )";
//        
//        std::cin >> choice;
//        std::cin.ignore(1000, '\n');
//        std::cout << std::endl;
//        
//        if (choice == 0) {
//            std::cout << "Thanks for playing!!!!!!\n";
//            break;
//        } else if (choice == 1) {
//            std::cout << "=============================\n";
//            std::cout << "Starting a game of 15 Puzzle.\n";
//            std::cout << "=============================\n";
//            FifteenPuzzle fp;
//            fp.runFifteenPuzzleGame();
//            
//        } else if (choice == 2) {
//            std::cout << "===========================\n";
//            std::cout << "Starting a game of Hangman.\n";
//            std::cout << "===========================\n";
//            Hangman hangman;
//            hangman.runHangmanGame();
//            
//        } else if (choice == 3) {
//            std::cout << "===================================\n";
//            std::cout << "Starting a game of Penguin Palooza.\n";
//            std::cout << "===================================\n";
//            runPenguinGame();
//            
//        } else if (choice == 4) {
//            std::cout << "=============================\n";
//            std::cout << "Starting a game of TicTacToe.\n";
//            std::cout << "=============================\n";
//            TicTacToe tictactoe;
//            tictactoe.runTicTacToe();
//        } else {
//            std::cout << "The feature is either not available or currently being developed\n";
//        }
//    }
