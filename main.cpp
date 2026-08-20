#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "FifteenPuzzle/FifteenPuzzle.hpp"
#include "PenguinPalooza/PenguinPalooza.hpp"
#include "PenguinPalooza/Game.hpp"
#include "Hangman/Hangman.hpp"
#include "TicTacToe/TicTacToe.hpp"
#include "GraphicsHelper.hpp"

int main() {
//    runPenguinGame();
//    return 0;
    
    enum class Screen {
        MainMenu,
        FifteenPuzzle,
        Hangman,
        PenguinPalooza,
        TicTacToe
    };
    Screen currentScreen = Screen::MainMenu;
    
    // Window creation
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "GameHub", sf::Style::Default);
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    
    // Text and buttons
    sf::Text title = createText(font, "The Game Hub", 80.f, window.getSize().x / 2.f, 50.f);
    
    sf::RectangleShape fifteenPuzzleButton = createButton({300.f, 80.f}, {window.getSize().x / 2.f - 150.f, 300.f}, 3.f);
    sf::Text fifteenPuzzleText = createText(font, "Fifteen Puzzle", 30.f, window.getSize().x / 2.f, 340.f);
    
    sf::RectangleShape hangmanButton = createButton({300.f, 80.f}, {window.getSize().x / 2.f - 150.f, 400.f}, 3.f);
    sf::Text hangmanText = createText(font, "Hangman", 30.f, window.getSize().x / 2.f, 440.f);
    
    sf::RectangleShape penguinButton = createButton({300.f, 80.f}, {window.getSize().x / 2.f - 150.f, 500.f}, 3.f);
    sf::Text penguinText = createText(font, "Penguin Palooza", 30.f, window.getSize().x / 2.f, 540.f);
    
    sf::RectangleShape ticTacToeButton = createButton({300.f, 80.f}, {window.getSize().x / 2.f - 150.f, 600.f}, 3.f);
    sf::Text ticTacToeText = createText(font, "Tic Tac Toe", 30.f, window.getSize().x / 2.f, 640.f);
    
    sf::RectangleShape quitButton = createButton({300.f, 80.f}, {window.getSize().x / 2.f - 150.f, 700.f}, 3.f);
    sf::Text quitText = createText(font, "Quit", 30.f, window.getSize().x / 2.f, 740.f);

    // Game objects
    TicTacToe ticTacToe;
    FifteenPuzzle fifteenPuzzle;
    Hangman hangman;
    Game penguinGame(10, 12, 50);
    
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
                    
                    if (fifteenPuzzleButton.getGlobalBounds().contains(mousePosition)) {
                        currentScreen = Screen::FifteenPuzzle;
                    } else if (hangmanButton.getGlobalBounds().contains(mousePosition)) {
                        currentScreen = Screen::Hangman;
                    } else if (penguinButton.getGlobalBounds().contains(mousePosition)) {
                        currentScreen = Screen::PenguinPalooza;
                    } else if (ticTacToeButton.getGlobalBounds().contains(mousePosition)) {
                        currentScreen = Screen::TicTacToe;
                    } else if (quitButton.getGlobalBounds().contains(mousePosition)) {
                        window.close();
                    }
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140));
        
        if (currentScreen == Screen::MainMenu) {
            window.draw(title);
            window.draw(fifteenPuzzleButton);
            window.draw(fifteenPuzzleText);
            window.draw(hangmanButton);
            window.draw(hangmanText);
            window.draw(penguinButton);
            window.draw(penguinText);
            window.draw(ticTacToeButton);
            window.draw(ticTacToeText);
            window.draw(quitButton);
            window.draw(quitText);
            
        } else if (currentScreen == Screen::FifteenPuzzle) {
            if (fifteenPuzzle.draw(window))
                currentScreen = Screen::MainMenu;
            
        } else if (currentScreen == Screen::Hangman) {
            if (hangman.draw(window))
                currentScreen = Screen::MainMenu;
            
        } else if (currentScreen == Screen::PenguinPalooza) {
            if (penguinGame.drawPenguinGame(window))
                currentScreen = Screen::MainMenu;
            
        } else if (currentScreen == Screen::TicTacToe) {
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
