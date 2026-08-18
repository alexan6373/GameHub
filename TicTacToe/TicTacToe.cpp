//    playerToMove
//    char board[3][3]

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "TicTacToe.hpp"

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

void TicTacToe::draw(sf::RenderWindow& window) {
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    
    sf::Text title(font, "TIC TAC TOE", 80);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
    title.setPosition({window.getSize().x / 2.f, 50});
    
    int move;
    
    const float boardSize = 570.f;
    const float cellSize = 190.f;
    
    float startX = window.getSize().x / 2.f - boardSize / 2.f;
    float startY = window.getSize().y / 2.f - boardSize / 2.f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    int mouseX = static_cast<float>(mouseButtonPressed->position.x);
                    int mouseY = static_cast<float>(mouseButtonPressed->position.y);
                    
                    
                    if (startX >= startX && startX < startX + boardSize &&
                        startY >= startY && mouseY < startY + boardSize) {
                        
                        move = 3 * (int) ((mouseY - startY) / cellSize) + (int) ((mouseX - startX) / cellSize);
                        
                        if (move < 0 || move > 8 || board[move / 3][move % 3] != ' ') {
                            std::cout << "Invalid move." << std::endl << std::endl;
                            continue;
                        } else {
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
                }
            }
        }

        window.clear();
        window.draw(title);
        
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape line({5.f, boardSize});
            line.setPosition({startX + i * cellSize, startY});
            window.draw(line);
        }
        
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape line({boardSize, 5.f});
            line.setPosition({startX, startY + i * cellSize});
            window.draw(line);
        }
        
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == 'X') {
                    sf::Text xTile(font, "X", 150);
                    sf::FloatRect bounds = xTile.getLocalBounds();
                    xTile.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
                    xTile.setPosition({startX + cellSize / 2 + row * cellSize, startY + cellSize / 2 + col * cellSize});
                    window.draw(xTile);
                    
                } else if (board[row][col] == 'O') {
                    sf::Text oTile(font, "O", 150);
                    sf::FloatRect bounds = oTile.getLocalBounds();
                    oTile.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
                    oTile.setPosition({startX + cellSize / 2 + row * cellSize, startY + cellSize / 2 + col * cellSize});
                    window.draw(oTile);
                }
            }
        }
        
        window.display();
    }
}
