#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Hangman.hpp"
#include "GraphicsHelper.hpp"
#include "AudioHelper.hpp"

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

Hangman::Hangman() : alreadyUsedLetters(26, 0), guessLeft(6), letterInSecretWord(26, false) {
    std::random_device rd;
    std::mt19937 mt(rd());
    
    secretWord = wordBank[mt() % wordBank.size()];
    for (int i = 0; i < secretWord.size(); i++)
        runningGuess.push_back('_');
    
    for (char ch : secretWord)
        letterInSecretWord[ch - 'a'] = true;
}

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

void Hangman::setKeyboardColor(sf::RectangleShape& keyboardTile, char letter) {
    if (alreadyUsedLetters[letter - 'a'] == 0) {
        keyboardTile.setFillColor(sf::Color::Transparent);
    } else if (letterInSecretWord[letter - 'a']) {
        keyboardTile.setFillColor(sf::Color::Green);
    } else {
        keyboardTile.setFillColor(sf::Color(150, 150, 150));
    }
}

bool Hangman::draw(sf::RenderWindow& window) {
    // --------------
    // Set Up Hangman
    // --------------
    gameOver = false;
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    const float windowWidth = window.getSize().x;
    const float fontSize = 20.f;
    const std::vector<std::vector<char>> keyboard = {
        {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
        {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
        {'z', 'x', 'c', 'v', 'b', 'n', 'm'}
    };
    
    sf::Text title = createText(font, "Hangman", 80.f, windowWidth / 2.f, 50.f);
    
    sf::RectangleShape menuButton = createButton({300.f, 60.f},
                                                 {windowWidth / 2.f - 150.f, 110.f}, 3.f);
    sf::Text menuText = createText(font, "Return to main menu", fontSize, windowWidth / 2.f, 140.f);
    
    sf::Text infoText = createText(font, "Guess a letter.", fontSize, windowWidth / 2.f, 210.f);
    
    sf::Text currGuessText = createText(font, "", fontSize, windowWidth / 2.f, 310.f);
    
    sf::Text guessesLeftText = createText(font, "You have " + std::to_string(guessLeft) + " guesses left.", fontSize, windowWidth / 2.f, 410.f);
    
    sf::Text alreadyGuessedText = createText(font, "You have already guessed: ", fontSize, windowWidth / 2.f, 510.f);
    
    char guess;

    // ---------------
    // Main Gamne Loop
    //----------------
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            // ---------------------------------
            // Handles Keyboard and Mouse Events
            // ---------------------------------
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    return true;
            }
            
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                if (gameOver)
                    continue;
                
                char32_t unicode = textEntered->unicode;
                guess = static_cast<char>(unicode);
                
                if (isValidGuess(guess)) {
                    hangmanMoveSound.play();
                    alreadyUsedLetters[tolower(guess) - 'a'] = 1;
                    if (guessIsCorrect(guess)) {
                        infoText.setString(std::string(1, toupper(guess)) + " is in the word.");
                        updateBoard(guess);
                    } else {
                        infoText.setString(std::string(1, toupper(guess)) + " is not in the word.");
                        guessLeft--;
                    }
                } else {
                    hangmanMoveSound.play();
                    infoText.setString("Invalid guess or you already guessed that letter.");
                }
                
                if (guessLeft == 0) {
                    infoText.setString("You ran out of guesses. The word was " + secretWord + ".");
                    defeatSound.play();
                    gameOver = true;
                } else if (didPlayerWin()) {
                    infoText.setString("Congratulations. You won.");
                    victorySound.play();
                    gameOver = true;
                }
            }
            
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    continue;
                
                float mouseX = static_cast<float>(mouseButtonPressed->position.x);
                float mouseY = static_cast<float>(mouseButtonPressed->position.y);
                
                if (menuButton.getGlobalBounds().contains({mouseX, mouseY})) {
                    clickSound.play();
                    return true;
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140));

        // -------------------
        // Render Game Objects
        // -------------------
        recenterText(infoText);
        
        std::string currentGuess = "";
        for (int i = 0; i < secretWord.size(); i++)
            currentGuess += std::string(1, toupper(runningGuess[i])) + " ";
        currGuessText.setString(currentGuess);
        recenterText(currGuessText);
        
        guessLeft == 1 ? guessesLeftText.setString("You have " + std::to_string(guessLeft) + " guess left.")
                       : guessesLeftText.setString("You have " + std::to_string(guessLeft) + " guesses left.");
        recenterText(guessesLeftText);
        
        std::string lettersGuessed = "You have already guessed";
//        for (int i = 0; i < 26; i++)
//            if (alreadyUsedLetters[i] == 1)
//                lettersGuessed += std::string(1, (char) ('A' + i)) + " ";
//        alreadyGuessedText.setString(lettersGuessed);
//        recenterText(alreadyGuessedText);
        
        for (int i = 0; i < 10; i++) {
            sf::RectangleShape keyboardTile = createButton({50.f, 50.f},
                                                           {windowWidth / 2.f - 250.f + i * 53.f, 600.f}, 3.f);
            sf::Text keyboardText = createText(font, std::string(1, (char) toupper(keyboard[0][i])), 30.f, windowWidth / 2.f - 225.f + i * 53.f, 625.f);
            
            setKeyboardColor(keyboardTile, keyboard[0][i]);
            window.draw(keyboardTile);
            window.draw(keyboardText);
        }
        
        for (int i = 0; i < 9; i++) {
            sf::RectangleShape keyboardTile = createButton({50.f, 50.f},
                                                       {windowWidth / 2.f - 225.f + i * 53.f, 653.f}, 3.f);
            sf::Text keyboardText = createText(font, std::string(1, (char) toupper(keyboard[1][i])), 30.f, windowWidth / 2.f - 200.f + i * 53.f, 678.f);
            setKeyboardColor(keyboardTile, keyboard[1][i]);
            window.draw(keyboardTile);
            window.draw(keyboardText);
        }
        
        for (int i = 0; i < 7; i++) {
            sf::RectangleShape keyboardTile = createButton({50.f, 50.f},
                                                       {windowWidth / 2.f - 200.f + i * 53.f, 706}, 3.f);
            sf::Text keyboardText = createText(font, std::string(1, (char) toupper(keyboard[2][i])), 30.f, windowWidth / 2.f - 175.f + i * 53.f, 731.f);
            setKeyboardColor(keyboardTile, keyboard[2][i]);
            window.draw(keyboardTile);
            window.draw(keyboardText);
        }
            
        window.draw(title);
        window.draw(menuText);
        window.draw(menuButton);
        window.draw(infoText);
        window.draw(currGuessText);
        window.draw(guessesLeftText);
        window.draw(alreadyGuessedText);
        
        window.display();
    }
    return true;
}
