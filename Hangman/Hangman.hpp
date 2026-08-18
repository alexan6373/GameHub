#ifndef HANGMAN_H
#define HANGMAN_H
#include <string>
#include <vector>

class Hangman {
private:
    std::string secretWord;
    std::vector<char> runningGuess;
    std::vector<int> alreadyUsedLetters;
    std::vector<std::string> wordBank = {
        "apple", "banana", "computer", "hangman", "elephant",
        "library", "mountain", "keyboard", "programming", "galaxy", "ocean",
        "penguin", "triangle", "volcano", "sunflower", "adventure",
        "chocolate", "rainbow", "astronaut", "notebook"
    };
    int guessLeft;
    
//    Helper functions
    void updateBoard(char guess);
    void displayBoard();
    bool isValidGuess(char guess);
    bool guessIsCorrect(char guess);
    bool didPlayerWin();

public:
    Hangman();
    void runHangmanGame();

};

#endif
