#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "FifteenPuzzle/FifteenPuzzle.hpp"
#include "PenguinPalooza/PenguinPalooza.hpp"
#include "Hangman/Hangman.hpp"
#include "TicTacToe/TicTacToe.hpp"

int main() {
    // Window creation
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "GameHub", sf::Style::Default);
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    
    enum class Screen {
        MainMenu,
        TicTacToe,
    };
    Screen currentScreen = Screen::MainMenu;
    
    // Main menu objects
    sf::Text title(font, "The Game Hub", 80);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    title.setPosition({window.getSize().x / 2.f, 50});
    
    sf::RectangleShape ticTacToeButton({300.f, 80.f});
    ticTacToeButton.setPosition({window.getSize().x / 2.f - 150.f, 300.f});
    ticTacToeButton.setFillColor(sf::Color::Transparent);
    ticTacToeButton.setOutlineColor(sf::Color::White);
    ticTacToeButton.setOutlineThickness(3.f);

    sf::Text ticTacToeText(font, "Tic Tac Toe", 30.f);
    sf::FloatRect textBounds = ticTacToeText.getLocalBounds();
    ticTacToeText.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
    ticTacToeText.setPosition({window.getSize().x / 2.f, 340.f});

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
        
        window.clear();
        
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
    
//    std::cout << R"(-----------------------------------------------------------------------------
//
// _______ _   _ ______    _____          __  __ ______    _    _ _    _ ____  
//|__   __| | | |  ____|  / ____|   /\   |  \/  |  ____|  | |  | | |  | |  _ \ 
//   | |  | |_| | |__    | |  __   /  \  | \  / | |__     | |__| | |  | | |_) |
//   | |  |  _  |  __|   | | |_ | / /\ \ | |\/| |  __|    |  __  | |  | |  _ < 
//   | |  | | | | |____  | |__| |/ ____ \| |  | | |____   | |  | | |__| | |_) |
//   |_|  |_| |_|______|  \_____/_/    \_\_|  |_|______|  |_|  |_|\____/|____/
//
//-----------------------------------------------------------------------------
//    )";
//    
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
    
    return 0;
}
