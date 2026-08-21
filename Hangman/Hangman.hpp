#ifndef HANGMAN_H
#define HANGMAN_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Hangman {
private:
    std::vector<std::string> wordBank = {
        "apple", "banana", "computer", "hangman", "elephant",
        "library", "mountain", "keyboard", "programming", "galaxy", "ocean",
        "penguin", "triangle", "volcano", "sunflower", "adventure",
        "chocolate", "rainbow", "astronaut", "notebook"
    };
    
    // Game State
    std::string secretWord;
    std::vector<char> runningGuess;
    std::vector<int> alreadyUsedLetters;
    std::vector<bool> letterInSecretWord;
    int guessLeft;
    bool gameOver;
    
    // Game Logic
    void updateBoard(char guess);
    void displayBoard();
    bool isValidGuess(char guess);
    bool guessIsCorrect(char guess);

public:
    Hangman();
    
    // Game Control
    void runHangmanGame();
    bool didPlayerWin();
    
    // Rendering The Game
    void setKeyboardColor(sf::RectangleShape& keyboardTile, char letter);
    bool draw(sf::RenderWindow& window);
};

#endif
