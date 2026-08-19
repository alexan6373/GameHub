#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "TicTacToe.hpp"
#include "GraphicsHelper.hpp"

void TicTacToe::updateBoard(int move) {
    board[move / 3][move % 3] = playerToMove;
}

void TicTacToe::displayBoard() {
    for (int row = 0; row < 3; row++) {
        std::cout << "-------------" << std::endl;
        for (int col = 0; col < 3; col++) {
            std::cout << "| ";
            if (board[row][col] == ' ') {
                std::cout << row * 3 + col << " ";
            } else {
                std::cout << board[row][col] << " ";
            }
        }
        std::cout << "|" << std::endl;
    }
    
    std::cout << "-------------" << std::endl;
}

bool TicTacToe::isWinner(char player) {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] &&
            board[row][1] == board[row][2] &&
            board[row][0] == player) {
            return true;
        }
    }

    // Check columns
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] &&
            board[1][col] == board[2][col] &&
            board[0][col] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2] &&
        board[0][0] == player) {
        return true;
    }

    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0] &&
        board[0][2] == player) {
        return true;
    }
    
    return false;
}

bool TicTacToe::isBoardFull() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                return false;
            }
        }
    }
    
    return true;
}

TicTacToe::TicTacToe() : playerToMove('X'),
    board({' ', ' ', ' '},
          {' ', ' ', ' '},
          {' ', ' ', ' '}) {}

void TicTacToe::runTicTacToe() {
    int move;
    while (true) {
        displayBoard();
        
        std::cout << "Player " << playerToMove << ". Make a move: ";
        std::cin >> move;
        std::cin.ignore(1000, '\n');
        
        if (move < 0 || move > 8 || board[move / 3][move % 3] != ' ') {
            std::cout << "Invalid move." << std::endl << std::endl;
            continue;
        }
        
        updateBoard(move);
        if (isWinner(playerToMove)) {
            std::cout << "The game is over. Player " << playerToMove << " won." << std::endl;
            break;
        } else if (isBoardFull()) {
            std::cout << "The game is a draw." << std::endl;
            break;
        }
        
        playerToMove = playerToMove == 'X' ? 'O' : 'X';
    }
}

bool TicTacToe::draw(sf::RenderWindow& window) {
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    
    sf::Text title = createText(font, "TIC TAC TOE", 80.f,
                               window.getSize().x / 2.f, 50);
    
    sf::RectangleShape menuButton = createButton({300.f, 60.f}, {window.getSize().x / 2.f - 150.f, 110.f}, 3.f);

    sf::Text menuText = createText(font, "Return to main menu", 20.f,
                                  window.getSize().x / 2.f, 140.f);
    
    sf::Text infoText = createText(font, "It is Player X's turn.", 20.f,
                                   window.getSize().x / 2.f, 850.f);
    
    int move;
    playerToMove = 'X';
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            board[row][col] = ' ';
        }
    }
    
    const float boardSize = 570.f;
    const float cellSize = 190.f;
    
    float startX = window.getSize().x / 2.f - boardSize / 2.f;
    float startY = window.getSize().y / 2.f - boardSize / 2.f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    continue;
                
                float mouseX = static_cast<float>(mouseButtonPressed->position.x);
                float mouseY = static_cast<float>(mouseButtonPressed->position.y);
                
                if (menuButton.getGlobalBounds().contains({mouseX, mouseY})) {
                    gameOver = false;
                    return true;
                }
                
                if (!gameOver && mouseX >= startX && mouseX < startX + boardSize &&
                    mouseY >= startY && mouseY < startY + boardSize) {
                    
                    int row = (mouseY - startY) / cellSize;
                    int col = (mouseX - startX) / cellSize;
                    move = 3 * row + col;
                    
                    if (move < 0 || move > 8 || board[move / 3][move % 3] != ' ') {
                        infoText.setString("Invalid move.");
                        continue;
                    }
                    
                    updateBoard(move);
                    
                    if (isWinner(playerToMove)) {
                        infoText.setString("The game is over. Player " + std::string(1, playerToMove) + " won.");
                        gameOver = true;
                    } else if (isBoardFull()) {
                        infoText.setString("The game is a draw.");
                        gameOver = true;
                    } else {
                        playerToMove = playerToMove == 'X' ? 'O' : 'X';
                        infoText.setString("It is player " + std::string(1, playerToMove) + "'s turn.");
                    }
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140));
        
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape line({5.f, boardSize});
            line.setPosition({startX + i * cellSize, startY});
            line.setFillColor(sf::Color::Black);
            window.draw(line);
        }
        
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape line({boardSize, 5.f});
            line.setPosition({startX, startY + i * cellSize});
            line.setFillColor(sf::Color::Black);
            window.draw(line);
        }
        
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == 'X') {
                    sf::Text xTile = createText(font, "X", 150.f,
                                                startX + cellSize / 2 + col * cellSize,
                                                startY + cellSize / 2 + row * cellSize);
                    window.draw(xTile);
                    
                } else if (board[row][col] == 'O') {
                    sf::Text oTile = createText(font, "O", 150.f,
                                                startX + cellSize / 2 + col * cellSize,
                                                startY + cellSize / 2 + row * cellSize);
                    window.draw(oTile);
                }
            }
        }

        window.draw(title);
        window.draw(menuText);
        window.draw(menuButton);
        window.draw(infoText);
        
        window.display();
    }
    
    gameOver = false;
    return true;
}
