#include <iostream>
#include <random>
#include <string>

#include "Hangman.hpp"
#include "GraphicsHelper.hpp"

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

bool Hangman::draw(sf::RenderWindow& window) {
    gameOver = false;
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    
    sf::Text title = createText(font, "Hangman", 80.f, window.getSize().x / 2.f, 50);
    
    sf::RectangleShape menuButton = createButton({300.f, 60.f}, {window.getSize().x / 2.f - 150.f, 110.f}, 3.f);
    sf::Text menuText = createText(font, "Return to main menu", 20.f, window.getSize().x / 2.f, 140.f);
    
    sf::Text infoText = createText(font, "Guess a letter.", 20.f, window.getSize().x / 2.f, 210.f);
    
    sf::Text currGuess = createText(font, "_ _ _ _ _ _ _ _ __ _ _ _ _ _ ", 20.f, window.getSize().x / 2.f, 310.f);
    
    sf::Text guessesLeft = createText(font, "You have " + std::to_string(guessLeft) + " guesses left.", 20.f, window.getSize().x / 2.f, 410.f);
    
    sf::Text alreadyGuessed = createText(font, "You have already guessed: ", 20.f, window.getSize().x / 2.f, 510.f);
    
    char guess;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
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
                    alreadyUsedLetters[tolower(guess) - 'a'] = 1;
                    if (guessIsCorrect(guess))
                        updateBoard(guess);
                    else {
                        infoText.setString(std::string(1, toupper(guess)) + " is not in the word.");
                        guessLeft--;
                    }
                } else {
                    infoText.setString("Invalid guess or you already guessed that letter.");
                }
                
                if (guessLeft == 0) {
                    infoText.setString("You ran out of guesses. The word was " + secretWord + ".");
                    gameOver = true;
                } else if (didPlayerWin()) {
                    infoText.setString("Congratulations. You won.");
                    gameOver = true;
                }
            }
            
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    continue;
                
                float mouseX = static_cast<float>(mouseButtonPressed->position.x);
                float mouseY = static_cast<float>(mouseButtonPressed->position.y);
                
                if (menuButton.getGlobalBounds().contains({mouseX, mouseY}))
                    return true;
            }
        }
        
        window.clear(sf::Color(210, 180, 140));

        recenterText(infoText);
        
        std::string currentGuess = "";
        for (int i = 0; i < secretWord.size(); i++)
            currentGuess += std::string(1, toupper(runningGuess[i])) + " ";
        currGuess.setString(currentGuess);
        recenterText(currGuess);
        
        if (guessLeft == 1)
            guessesLeft.setString("You have " + std::to_string(guessLeft) + " guess left.");
        else
            guessesLeft.setString("You have " + std::to_string(guessLeft) + " guesses left.");
        
        recenterText(guessesLeft);
        
        std::string lettersGuessed = "You have already guessed: ";
        for (int i = 0; i < 26; i++)
            if (alreadyUsedLetters[i] == 1)
                lettersGuessed += std::string(1, (char) ('A' + i)) + " ";
        alreadyGuessed.setString(lettersGuessed);
        recenterText(alreadyGuessed);
        
        window.draw(title);
        window.draw(menuText);
        window.draw(menuButton);
        window.draw(infoText);
        window.draw(currGuess);
        window.draw(guessesLeft);
        window.draw(alreadyGuessed);
        
        window.display();
    }
    return true;
}
