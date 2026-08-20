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
    enum class Screen {MainMenu, FifteenPuzzle, Hangman,
                       PenguinPalooza, TicTacToe};
    Screen currentScreen = Screen::MainMenu;
    
    // --------------------
    // Create Window Object
    // --------------------
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "GameHub", sf::Style::Default);
    
    // -----------------------
    // Create Text and Buttons
    // -----------------------
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    const float buttonWidth = 300.f;
    const float buttonHeight = 80.f;
    const float windowWidth = window.getSize().x;
    const float borderThickness = 3.f;
    const float buttonFontSize = 30.f;
    
    sf::Text title = createText(font, "The Game Hub", 80.f, windowWidth / 2.f, 50.f);
    
    sf::RectangleShape fifteenPuzzleButton = createButton({buttonWidth, buttonHeight},
                                                          {windowWidth / 2 - buttonWidth / 2, 300.f}, borderThickness);
    sf::Text fifteenPuzzleText = createText(font, "Fifteen Puzzle", buttonFontSize, windowWidth / 2.f, 340.f);
    
    sf::RectangleShape hangmanButton = createButton({buttonWidth, buttonHeight},
                                                    {windowWidth / 2 - buttonWidth / 2, 400.f}, borderThickness);
    sf::Text hangmanText = createText(font, "Hangman", buttonFontSize, windowWidth / 2.f, 440.f);
    
    sf::RectangleShape penguinButton = createButton({buttonWidth, buttonHeight},
                                                    {windowWidth / 2 - buttonWidth / 2, 500.f}, borderThickness);
    sf::Text penguinText = createText(font, "Penguin Palooza", buttonFontSize, windowWidth / 2.f, 540.f);
    
    sf::RectangleShape ticTacToeButton = createButton({buttonWidth, buttonHeight},
                                                      {windowWidth / 2 - buttonWidth / 2, 600.f}, borderThickness);
    sf::Text ticTacToeText = createText(font, "Tic Tac Toe", buttonFontSize, windowWidth / 2.f, 640.f);
    
    sf::RectangleShape quitButton = createButton({buttonWidth, buttonHeight},
                                                 {windowWidth / 2 - buttonWidth / 2, 700.f}, borderThickness);
    sf::Text quitText = createText(font, "Quit", buttonFontSize, windowWidth / 2.f, 740.f);

    // -------------------
    // Create Game Objects
    // -------------------
    TicTacToe ticTacToe;
    FifteenPuzzle fifteenPuzzle;
    Hangman hangman;
    Game penguinGame(10, 12, 50);
    
    while (window.isOpen()) {
        // ---------------------------------
        // Handles Keyboard and Mouse Events
        // ---------------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePosition(static_cast<float>(mouseButtonPressed->position.x),
                                               static_cast<float>(mouseButtonPressed->position.y));
                    
                    if (fifteenPuzzleButton.getGlobalBounds().contains(mousePosition))
                        currentScreen = Screen::FifteenPuzzle;
                    else if (hangmanButton.getGlobalBounds().contains(mousePosition))
                        currentScreen = Screen::Hangman;
                    else if (penguinButton.getGlobalBounds().contains(mousePosition))
                        currentScreen = Screen::PenguinPalooza;
                    else if (ticTacToeButton.getGlobalBounds().contains(mousePosition))
                        currentScreen = Screen::TicTacToe;
                    else if (quitButton.getGlobalBounds().contains(mousePosition))
                        window.close();
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140)); // Tan colored background
        
        // -----------------------
        // Render Text and Buttons
        // -----------------------
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
