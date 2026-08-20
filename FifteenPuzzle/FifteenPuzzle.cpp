#include <iostream>
#include <random>
#include <string>

#include "FifteenPuzzle.hpp"
#include "GraphicsHelper.hpp"

// Helper functions
void FifteenPuzzle::makeMove(std::string move) {
    int x = emptySquare.first;
    int y = emptySquare.second;
    
    if (move == "w") {
        board[x][y] = board[x + 1][y];
        board[x + 1][y] = 0;
        emptySquare.first++;
    } else if (move == "s") {
        board[x][y] = board[x - 1][y];
        board[x - 1][y] = 0;
        emptySquare.first--;
    } else if (move == "a") {
        board[x][y] = board[x][y + 1];
        board[x][y + 1] = 0;
        emptySquare.second++;
    } else if (move == "d") {
        board[x][y] = board[x][y - 1];
        board[x][y - 1] = 0;
        emptySquare.second--;
    }
}

bool FifteenPuzzle::isValidMove(std::string move) {
    if (move != "w" && move != "s" && move != "a" && move != "d")
        return false;
    else if (move == "w" && emptySquare.first == 3)
        return false;
    else if (move == "s" && emptySquare.first == 0)
        return false;
    else if (move == "a" && emptySquare.second == 3)
        return false;
    else if (move == "d" && emptySquare.second == 0l)
        return false;
    
    return true;
}

bool FifteenPuzzle::isGameOver() {
    int val = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((i != 3 || j != 3) && board[i][j] != val)
                return false;
            val++;
        }
    }
    
    return true;
}

void FifteenPuzzle::displayBoard() {
    std::cout << "---------------------\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << "| ";
            
            if (board[i][j] < 10)
                std::cout << " ";
            
            if (board[i][j] == 0)
                std::cout << "X ";
            else
                std::cout << board[i][j] << " ";
        }
        std::cout << "| " << std::endl;
    }
    std::cout << "---------------------\n";
}

void FifteenPuzzle::setUpBoard() {
    emptySquare = {3, 3};
    
    int num = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i != emptySquare.first || j != emptySquare.second) {
                board[i][j] = num;
                num++;
            }
        }
    }
    
    std::random_device rd;
    std::mt19937 mt(rd());
    
    for (int i = 0; i < 1000; i++) {
        int move = mt() % 4;
        std::string moves[4] = {"w", "a", "s", "d"};
        
        if (isValidMove(moves[move]))
            makeMove(moves[move]);
            
    }
}
    
// Constructor
FifteenPuzzle::FifteenPuzzle():
    board{{1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}},
    emptySquare{3, 3} {
}

// Main game functions
void FifteenPuzzle::runFifteenPuzzleGame() {
    setUpBoard();
    
    while(true) {
        displayBoard();
        
        if (isGameOver()) {
            std::cout << "You have won. Congratulations.\n" << std::endl;
            std::cout << "Play again? y/n: ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "y") {
                setUpBoard();
                displayBoard();
            } else {
                std::cout << "Thanks for playing 15 Puzzle." << std::endl;
                break;
            }
        }
        
        std::string move = "";
        
        std::cout << "Moves\n";
        std::cout << "w: Up\n";
        std::cout << "s: Down\n";
        std::cout << "a: Left\n";
        std::cout << "d: Right\n";
        std::cout << "Enter q to quit the game.\n";
        std::cout << "Enter a move: ";
        std::getline(std::cin, move);
        
        if (move == "q") {
            break;
        } else if (!isValidMove(move)) {
            std::cout << "Invalid Move\n\n";
        } else {
            makeMove(move);
        }
    }
}

bool FifteenPuzzle::draw(sf::RenderWindow& window) {
    setUpBoard();
    gameOver = false;
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    sf::Text title = createText(font, "Fifteen Puzzle", 80.f, window.getSize().x / 2.f, 50);
    
    sf::RectangleShape menuButton = createButton({300.f, 60.f}, {window.getSize().x / 2.f - 150.f, 110.f}, 3.f);
    sf::Text menuText = createText(font, "Return to main menu", 20.f, window.getSize().x / 2.f, 140.f);
    
    sf::Text infoText = createText(font, "Use WASD or arrow keys to make moves.", 20.f, window.getSize().x / 2.f, 850.f);
    
    std::string move = "";
    const float boardSize = 600.f;
    const float cellSize = 150.f;
    
    float startX = window.getSize().x / 2.f - boardSize / 2.f + 10;
    float startY = window.getSize().y / 2.f - boardSize / 2.f + 10;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    continue;
                
                float mouseX = static_cast<float>(mouseButtonPressed->position.x);
                float mouseY = static_cast<float>(mouseButtonPressed->position.y);
                
                if (menuButton.getGlobalBounds().contains({mouseX, mouseY}))
                    return true;
            
            }
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (!gameOver && (keyPressed->scancode == sf::Keyboard::Scancode::W ||
                                  keyPressed->scancode == sf::Keyboard::Scancode::Up))
                    move = "w";
                else if (!gameOver && (keyPressed->scancode == sf::Keyboard::Scancode::A ||
                                       keyPressed->scancode == sf::Keyboard::Scancode::Left))
                    move = "a";
                else if (!gameOver && (keyPressed->scancode == sf::Keyboard::Scancode::S ||
                                       keyPressed->scancode == sf::Keyboard::Scancode::Down))
                    move = "s";
                else if (!gameOver && (keyPressed->scancode == sf::Keyboard::Scancode::D ||
                                       keyPressed->scancode == sf::Keyboard::Scancode::Right))
                    move = "d";
            
                
                if (!gameOver && !isValidMove(move)) {
                    infoText.setString("Invalid move.");
                } else if (!gameOver && isValidMove(move)) {
                    makeMove(move);
                    infoText.setString("Use WASD or arrow keys to make moves.");
                    
                    if (isGameOver()) {
                        infoText.setString("You have won. Congratulations.\n");
                        gameOver = true;
                    }
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140));
        
        for (int i = 0; i < 5; i++) {
            sf::RectangleShape line({5.f, boardSize});
            line.setPosition({startX + i * cellSize, startY});
            line.setFillColor(sf::Color::Black);
            window.draw(line);
        }
        
        for (int i = 0; i < 5; i++) {
            sf::RectangleShape line({boardSize, 5.f});
            line.setPosition({startX, startY + i * cellSize});
            line.setFillColor(sf::Color::Black);
            window.draw(line);
        }
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                int cellVal = board[row][col];
                if (board[row][col] != 0) {
                    sf::Text tile = createText(font, std::to_string(cellVal), 120.f,
                                               startX + cellSize / 2 + col * cellSize,
                                               startY + cellSize / 2 + row * cellSize);
                    
                    if (cellVal == 4 * row + col + 1)
                        tile.setFillColor(sf::Color::Green);
                    else
                        tile.setFillColor(sf::Color::Blue);
                    
                    window.draw(tile);
                    
                }
            }
        }
        
        recenterText(infoText);

        window.draw(title);
        window.draw(menuText);
        window.draw(menuButton);
        window.draw(infoText);
        
        window.display();
    }
    
    return true;
}
