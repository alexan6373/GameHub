#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "FifteenPuzzle/FifteenPuzzle.hpp"
#include "PenguinPalooza/PenguinPalooza.hpp"
#include "PenguinPalooza/Game.hpp"
#include "Hangman/Hangman.hpp"
#include "TicTacToe/TicTacToe.hpp"
#include "ChorusLapilli/ChorusLapilli.hpp"

#include "GraphicsHelper.hpp"
#include "AudioHelper.hpp"

int main() {
    enum class Screen {MainMenu, FifteenPuzzle, Hangman,
                       PenguinPalooza, TicTacToe, ChorusLapilli};
    Screen currentScreen = Screen::MainMenu;
    
    // --------------------
    // Create Window Object
    // --------------------
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "GameHub", sf::Style::Default);
    
    // -----------------------
    // Create Text and Buttons
    // -----------------------
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    const float windowWidth = window.getSize().x;
    const float windowHeight = window.getSize().y;
    
    const float titleHeight = windowHeight * 0.08f;
    const float buttonWidth = windowWidth * 0.25f;
    const float buttonHeight = windowHeight * 0.08f;
    const float borderThickness = 3.f;
    const float buttonFontSize = windowHeight * 0.03f;
    
    sf::Text title = createText(font, "The Game Hub", titleHeight, windowWidth / 2.f, windowHeight * 0.1f);
    
    sf::RectangleShape fifteenPuzzleButton = createButton({buttonWidth, buttonHeight},
                                                          {windowWidth / 2 - buttonWidth / 2, windowHeight * 0.2f},
                                                          borderThickness);
    fifteenPuzzleButton.setFillColor(sf::Color(245, 230, 204));
    sf::Text fifteenPuzzleText = createText(font, "Fifteen Puzzle", buttonFontSize,
                                            windowWidth / 2.f, windowHeight * 0.24f);
    
    sf::RectangleShape hangmanButton = createButton({buttonWidth, buttonHeight},
                                                    {windowWidth / 2 - buttonWidth / 2, windowHeight * 0.3f},
                                                    borderThickness);
    hangmanButton.setFillColor(sf::Color(13, 27, 42));
    sf::Text hangmanText = createText(font, "Hangman", buttonFontSize,
                                      windowWidth / 2.f, windowHeight * 0.34f);
    hangmanText.setFillColor(sf::Color(241, 245, 249));
    
    sf::RectangleShape penguinButton = createButton({buttonWidth, buttonHeight},
                                                    {windowWidth / 2 - buttonWidth / 2, windowHeight * 0.4f},
                                                    borderThickness);
    penguinButton.setFillColor(sf::Color(244, 248, 251));
    sf::Text penguinText = createText(font, "Penguin Palooza", buttonFontSize,
                                      windowWidth / 2.f, windowHeight * 0.44f);
    
    sf::RectangleShape ticTacToeButton = createButton({buttonWidth, buttonHeight},
                                                      {windowWidth / 2 - buttonWidth / 2, windowHeight * 0.5f},
                                                      borderThickness);
    ticTacToeButton.setFillColor(sf::Color(207, 232, 169));
    sf::Text ticTacToeText = createText(font, "Tic Tac Toe", buttonFontSize,
                                        windowWidth / 2.f, windowHeight * 0.54f);
    
    sf::RectangleShape chorusLapilliButton = createButton({buttonWidth, buttonHeight},
                                                          {windowWidth / 2 - buttonWidth / 2, windowHeight * 0.6f},
                                                          borderThickness);
    chorusLapilliButton.setFillColor(sf::Color(232, 211, 165));
    sf::Text chorusLapilliText = createText(font, "Chorus Lapilli", buttonFontSize,
                                            windowWidth / 2.f, windowHeight * 0.64f);
    
    sf::RectangleShape quitButton = createButton({buttonWidth, buttonHeight},
                                                 {windowWidth / 2 - buttonWidth / 2, windowHeight * 0.7f},
                                                 borderThickness);
    quitButton.setFillColor(sf::Color::Red);
    sf::Text quitText = createText(font, "Quit", buttonFontSize,
                                   windowWidth / 2.f, windowHeight * 0.74f);
    
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
                    
                    if (fifteenPuzzleButton.getGlobalBounds().contains(mousePosition)) {
                        clickSound.play();
                        currentScreen = Screen::FifteenPuzzle;
                    } else if (hangmanButton.getGlobalBounds().contains(mousePosition)) {
                        clickSound.play();
                        currentScreen = Screen::Hangman;
                    } else if (penguinButton.getGlobalBounds().contains(mousePosition)) {
                        clickSound.play();
                        currentScreen = Screen::PenguinPalooza;
                    } else if (ticTacToeButton.getGlobalBounds().contains(mousePosition)) {
                        clickSound.play();
                        currentScreen = Screen::TicTacToe;
                    } else if (chorusLapilliButton.getGlobalBounds().contains(mousePosition)) {
                        clickSound.play();
                        currentScreen = Screen::ChorusLapilli;
                    } else if (quitButton.getGlobalBounds().contains(mousePosition)) {
                        clickSound.play();
                        window.close();
                    }
                }
            }
        }
        
        // Light brown color background
        window.clear(sf::Color(160, 130, 90));
        
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
            window.draw(chorusLapilliButton);
            window.draw(chorusLapilliText);
            window.draw(quitButton);
            window.draw(quitText);
            
        } else if (currentScreen == Screen::FifteenPuzzle) {
            FifteenPuzzle fifteenPuzzle;
            if (fifteenPuzzle.draw(window))
                currentScreen = Screen::MainMenu;
            
        } else if (currentScreen == Screen::Hangman) {
            Hangman hangman;
            if (hangman.draw(window))
                currentScreen = Screen::MainMenu;
            
        } else if (currentScreen == Screen::PenguinPalooza) {
            Game penguinGame(10, 12, 50);
            if (penguinGame.draw(window))
                currentScreen = Screen::MainMenu;
            
        } else if (currentScreen == Screen::TicTacToe) {
            TicTacToe ticTacToe;
            if (ticTacToe.draw(window))
                currentScreen = Screen::MainMenu;
        
        } else if (currentScreen == Screen::ChorusLapilli) {
            ChorusLapilli chorusLapilli;
            if (chorusLapilli.draw(window))
                currentScreen = Screen::MainMenu;
        }
        
        window.display();
    }
    return 0;
}
