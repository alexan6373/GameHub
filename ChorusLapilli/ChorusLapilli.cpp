#include "ChorusLapilli.hpp"
#include "GraphicsHelper.hpp"
#include "AudioHelper.hpp"
bool ChorusLapilli::isWinner(char player) {
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

bool ChorusLapilli::isValidMove(int start, int end) {
    if (board[end / 3][end % 3] != ' ')
        return false;

    for (int destination : legalMoves[start]) {
        if (destination == end)
            return true;
    }

    return false;
}

ChorusLapilli::ChorusLapilli() : playerToMove('X'),
    board({' ', ' ', ' '},
          {' ', ' ', ' '},
          {' ', ' ', ' '}),
    legalMoves{{1, 3, 4},
               {0, 2, 3, 4, 5},
               {1, 4, 5},
               {0, 1, 4, 6, 7},
               {0, 1, 2, 3, 5, 6, 7, 8},
               {1, 2, 4, 7, 8},
               {3, 4, 7},
               {3, 4, 5, 6, 8},
               {4, 5, 7}} {}

bool ChorusLapilli::draw(sf::RenderWindow& window) {
    // ------------------
    // Set Up Tic Tac Toe
    // ------------------
    gameOver = false;
    enum class MoveType {Setup, Select, Move};
    MoveType currMove = MoveType::Setup;
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    const float windowWidth = window.getSize().x;
    
    sf::Text title = createText(font, "Chorus Laipilli", 80.f, windowWidth / 2.f, 50.f);
    
    sf::RectangleShape menuButton = createButton({300.f, 60.f},
                                                 {windowWidth / 2.f - 150.f, 110.f}, 3.f);
    sf::Text menuText = createText(font, "Return to main menu", 20.f, windowWidth / 2.f, 140.f);
    
    sf::Text infoText = createText(font, "It is Player X's turn.", 20.f, windowWidth / 2.f, 850.f);
    
    int moveStart = -1;
    int moveEnd = -1;
    int movesSoFar = 0;
    
    playerToMove = 'X';
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            board[row][col] = ' ';
    
    const float boardSize = 570.f;
    const float cellSize = 190.f;
    
    // Represents bounds of the board
    const float startX = window.getSize().x / 2.f - boardSize / 2.f;
    const float startY = window.getSize().y / 2.f - boardSize / 2.f;
    const float endX = startX + boardSize;
    const float endY = startY + boardSize;

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
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    continue;
                
                float mouseX = static_cast<float>(mouseButtonPressed->position.x);
                float mouseY = static_cast<float>(mouseButtonPressed->position.y);
                
                if (menuButton.getGlobalBounds().contains({mouseX, mouseY})) {
                    clickSound.play();
                    return true;
                }
                
                if (gameOver ||
                    mouseX < startX || mouseX >= endX ||
                    mouseY < startY || mouseY >= endY)
                    continue;
                
                int row = (mouseY - startY) / cellSize;
                int col = (mouseX - startX) / cellSize;
                
                if (currMove == MoveType::Setup) {
                    moveStart = 3 * row + col;
                    
                    if (board[moveStart / 3][moveStart % 3] != ' ') {
                        infoText.setString("Invalid move.");
                        continue;
                    }
                    
                    chorusLapilliSound.play();
                    board[moveStart / 3][moveStart % 3] = playerToMove;
                    
                    movesSoFar++;
                    if (movesSoFar >= 6) {
                        currMove = MoveType::Select;
                        moveStart = -1;
                    }
                    
                    if (isWinner(playerToMove)) {
                        infoText.setString("The game is over. Player " + std::string(1, playerToMove) + " won.");
                        victorySound.play();
                        gameOver = true;
                    } else {
                        playerToMove = playerToMove == 'X' ? 'O' : 'X';
                        infoText.setString("It is player " + std::string(1, playerToMove) + "'s turn.");
                    }
                    
                } else if (currMove == MoveType::Select) {
                    moveStart = 3 * row + col;
                    
                    if (moveStart < 0 || moveStart > 8 ||
                        board[moveStart / 3][moveStart % 3] != playerToMove) {
                        infoText.setString("Select your own tile, player " + std::string(1, playerToMove) + ".");
                        continue;
                    } else {
                        infoText.setString("Select a destination tile.");
                        currMove = MoveType::Move;
                    }
                
                } else if (currMove == MoveType::Move) {
                    moveEnd = 3 * row + col;
                    
                    if (!isValidMove(moveStart, moveEnd)) {
                        infoText.setString("Move to an empty adjacent square.");
                        currMove = MoveType::Select;
                        moveStart = -1;
                        moveEnd = -1;
                        continue;
                    }
                    
                    if (board[1][1] != playerToMove) {
                        board[moveStart / 3][moveStart % 3] = ' ';
                        board[moveEnd / 3][moveEnd % 3] = playerToMove;
                        
                        if (isWinner(playerToMove)) {
                            infoText.setString("The game is over. Player " + std::string(1, playerToMove) + " won.");
                            victorySound.play();
                            gameOver = true;
                        }
                        
                        playerToMove = playerToMove == 'X' ? 'O' : 'X';
                        infoText.setString("It is player " + std::string(1, playerToMove) + "'s turn.");
                        
                        currMove = MoveType::Select;
                        moveStart = -1;
                        moveEnd = -1;
                        continue;
                    }
                    
                    board[moveStart / 3][moveStart % 3] = ' ';
                    board[moveEnd / 3][moveEnd % 3] = playerToMove;
                    
                    if (board[1][1] == playerToMove && !isWinner(playerToMove)) {
                        board[moveStart / 3][moveStart % 3] = playerToMove;
                        board[moveEnd / 3][moveEnd % 3] = ' ';
                        
                        infoText.setString("You must vacate the center or win on your next move, player " + std::string(1, playerToMove) + ".");
                        
                    } else if (isWinner(playerToMove)) {
                        infoText.setString("The game is over. Player " + std::string(1, playerToMove) + " won.");
                        victorySound.play();
                        gameOver = true;
                    } else {
                        playerToMove = playerToMove == 'X' ? 'O' : 'X';
                        infoText.setString("It is player " + std::string(1, playerToMove) + "'s turn.");
                    }
                    
                    currMove = MoveType::Select;
                    moveStart = -1;
                    moveEnd = -1;
                }
            }
        }
        
        window.clear(sf::Color(232, 211, 165));
        
        // -------------------
        // Render Game Objects
        // -------------------
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sf::RectangleShape boardTile = createButton({cellSize, cellSize},
                                                            {startX + j * (cellSize + 3.f), startY + i * (cellSize + 3.f)},
                                                            3.f);
                
                if (currMove == MoveType::Move) {
                    if (3 * i + j == moveStart)
                        boardTile.setFillColor(sf::Color(217, 164, 65));
                    
                    if (isValidMove(moveStart, 3 * i + j) && board[i][j] == ' ')
                        boardTile.setFillColor(sf::Color(168, 182, 108));
                    
                }
                
                window.draw(boardTile);
            }
        }
        
        
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                char cellVal = board[row][col];
                if (cellVal != ' ') {
                    sf::Text tile = createText(font, std::string(1, cellVal), 150.f,
                                                startX + cellSize / 2 + col * cellSize,
                                                startY + cellSize / 2 + row * cellSize);
                    cellVal == 'X' ? tile.setFillColor(sf::Color(181, 82, 57))
                                   : tile.setFillColor(sf::Color(63, 95, 122));
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
