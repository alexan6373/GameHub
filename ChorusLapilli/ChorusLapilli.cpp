#include "ChorusLapilli.hpp"
#include "GraphicsHelper.hpp"
#include "AudioHelper.hpp"

void ChorusLapilli::updateBoard(int move) {
    board[move / 3][move % 3] = playerToMove;
}

ChorusLapilli::ChorusLapilli() : playerToMove('X'),
    board({' ', ' ', ' '},
          {' ', ' ', ' '},
          {' ', ' ', ' '}) {}

bool ChorusLapilli::draw(sf::RenderWindow& window) {
    // ------------------
    // Set Up Tic Tac Toe
    // ------------------
    gameOver = false;
    enum class MoveType {Setup, Select, Move};
    MoveType currMove = MoveType::Setup;
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    const float windowWidth = window.getSize().x;
    
    sf::Text title = createText(font, "Tic Tac Toe", 80.f, windowWidth / 2.f, 50.f);
    
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
                    updateBoard(moveStart);
                    
                    movesSoFar++;
                    if (movesSoFar >= 6)
                        currMove = MoveType::Select;
                    
                    playerToMove = playerToMove == 'X' ? 'O' : 'X';
                    infoText.setString("It is player " + std::string(1, playerToMove) + "'s turn.");

//                    if (isWinner(playerToMove)) {
//                        infoText.setString("The game is over. Player " + std::string(1, playerToMove) + " won.");
//                        victorySound.play();
//                        gameOver = true;
//                    } else if (isBoardFull()) {
//                        infoText.setString("The game is a draw.");
//                        gameOver = true;
//                        defeatSound.play();
//                    } else {
                        
//                    }
                } else if (currMove == MoveType::Select) {
                    moveStart = 3 * row + col;
                    
                    if (moveStart < 0 || moveStart > 8 ||
                        board[moveStart / 3][moveStart % 3] != ' ') {
                        infoText.setString("Invalid move.");
                        continue;
                    } else {
                        infoText.setString("Select a destination tile.");
                        currMove = MoveType::Move;
                    }
                } else if (currMove == MoveType::Move) {
                    
                }
            }
        }
        
        window.clear(sf::Color(210, 180, 140));
        
        // -------------------
        // Render Game Objects
        // -------------------
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sf::RectangleShape boardTile = createButton({cellSize, cellSize},
                                                            {startX + j * (cellSize + 3.f), startY + i * (cellSize + 3.f)},
                                                            3.f);
                if (3 * i + j == moveStart)
                    boardTile.setFillColor(sf::Color::Green);
                
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
                    cellVal == 'X' ? tile.setFillColor(sf::Color::Blue)
                                           : tile.setFillColor(sf::Color::Red);
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
