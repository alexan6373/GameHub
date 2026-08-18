#include <iostream>
#include <random>
#include <string>
#include "Hangman.hpp"

//    Helper functions
void Hangman::updateBoard(char guess) {
    for (int i = 0; i < secretWord.size(); i++) {
        if (secretWord[i] == guess)
            runningGuess[i] = guess;
    }
}
 
void Hangman::displayBoard() {
    for (int i = 0; i < secretWord.size(); i++)
        std::cout << toupper(runningGuess[i]) << " ";
    
    std::cout << std::endl;
    
    std::cout << "You have " << guessLeft << " guess(es) left.\n";
    std::cout << "You have already guessed: ";
    
    for (int i = 0; i < 26; i++)
        if (alreadyUsedLetters[i] == 1)
            std::cout << (char) ('A' + i) << " ";
    
    std::cout << "\n\n";
}

bool Hangman::isValidGuess(char guess) {
    return (toascii(guess) >= 'a' && tolower(guess) <= 'z' &&
            alreadyUsedLetters[tolower(guess) - 'a'] == 0);
}

bool Hangman::didPlayerWin() {
    for (int i = 0; i < runningGuess.size(); i++) {
        if (runningGuess[i] == '_')
            return false;
    }
    return true;
}


bool Hangman::guessIsCorrect(char guess) {
    for (int i = 0; i < secretWord.size(); i++) {
        if (secretWord[i] == guess)
            return true;
    }
    return false;
}

//    Constructor
Hangman::Hangman() : alreadyUsedLetters(26, 0), guessLeft(6) {
    std::random_device rd;
    std::mt19937 mt(rd());
    
    secretWord = wordBank[mt() % wordBank.size()];
    for (int i = 0; i < secretWord.size(); i++)
        runningGuess.push_back('_');
}


//    Main game function
void Hangman::runHangmanGame() {
    while(true) {
        displayBoard();
        if (guessLeft == 0) {
            std::cout << "You ran out of guesses.\n";
            std::cout << "The word was " << secretWord << "." << std::endl;
            break;
        }
        
        char guess = 0;
        std::cout << "Guess a letter: ";
        std::cin >> guess;
        std::cin.ignore(1000, '\n');
        
        if (isValidGuess(guess)) {
            alreadyUsedLetters[tolower(guess) - 'a'] = 1;
            if (guessIsCorrect(guess))
                updateBoard(guess);
            else {
                std::cout << "That letter is not in the word.\n";
                guessLeft--;
            }
        } else {
            std::cout << "Invalid guess or you already guessed that letter.\n\n";
        }
        
        if (didPlayerWin()) {
            std::cout << "Congratulations, you won. Thanks for playing Hangman.\n";
            break;
        }
    }
}
