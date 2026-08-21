#include "Game.hpp"
#include "Valley.hpp"
#include "Player.hpp"
#include "Penguin.hpp"
#include "PenguinGameHelper.hpp"
#include "GraphicsHelper.hpp"
#include "AudioHelper.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nPenguins) {
    if (nPenguins < 0  ||  nPenguins > MAXPENGUINS) {
        std::cout << "***** Game created with invalid number of penguins:  "
             << nPenguins << std::endl;
        exit(1);
    }
    
    int nEmpty = rows * cols - 1;  // 1 for Player
    
    if (nPenguins > 0)
        nEmpty--;  // All penguins could be on same spot if necessary
    if (nEmpty < 0) {
        std::cout << "***** Game created with a " << rows << " by "
             << cols << " valley, which is too small to hold a player and "
             << nPenguins << " penguins!" << std::endl;
        std::exit(1);
    }

//   Create valley
    m_valley = new Valley(rows, cols);

//    Add some protrusions in PROTRUSION_DENSITY of the empty spots
    assert(PROTRUSION_DENSITY >= 0  &&  PROTRUSION_DENSITY <= 1);
    int nProtrusions = static_cast<int>(PROTRUSION_DENSITY * nEmpty);
    while (nProtrusions > 0  &&  nEmpty > 0) {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_valley->hasProtrusionAt(r, c))
            continue;
        m_valley->placeProtrusionAt(r, c);
        nProtrusions--;
        nEmpty--;
    }

//   Add player
    int rPlayer;
    int cPlayer;
    do {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_valley->hasProtrusionAt(rPlayer, cPlayer));
    m_valley->addPlayer(rPlayer, cPlayer);

//   Populate with penguins
    while (nPenguins > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_valley->hasProtrusionAt(r, c)  ||  (r == rPlayer && c == cPlayer))
            continue;
        const char species[MAXSPECIES] = { 'G', 'M', 'K' };
        m_valley->addPenguin(r, c, species[randInt(1, MAXSPECIES)-1]);
        nPenguins--;
    }
}

Game::~Game() {
    delete m_valley;
}

std::string Game::takePlayerTurn() {
    for (;;) {
        std::cout << "Your move (n/e/s/w/x or nothing or q for quit): ";
        std::string playerMove;
        getline(std::cin, playerMove);

        Player* player = m_valley->player();
        int dir;

        if (playerMove.size() == 0) {
            if (recommendMove(*m_valley, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->stand();
            
        } else if (playerMove.size() == 1) {
            if (tolower(playerMove[0]) == 'x')
                return player->stand();
            else {
                dir = decodeDirection(tolower(playerMove[0]));
                if (dir != BADDIR)
                    return player->move(dir);
            }
        }
        std::cout << "Player move must be nothing, or 1 character n/e/s/w/x." << std::endl;
    }
}

std::string Game::takePenguinsTurn() {
    for (;;) {
        std::cout << "Fish type and direction to throw (e.g., Gn or kw): ";
        std::string typeAndDir;
        getline(std::cin, typeAndDir);
        
        if (typeAndDir.size() != 2) {
            std::cout << "You must specify a fish type followed by a direction." << std::endl;
            continue;
        }
        
        char type = toupper(typeAndDir[0]);
        if (type != 'G'  &&  type != 'M'  &&  type != 'K') {
            std::cout << "Fish type must be upper or lower G, M, or K" << "." << std::endl;
            continue;
        }
        
        int dir = decodeDirection(tolower(typeAndDir[1]));
        if (dir == BADDIR) {
            std::cout << "Direction must be n, e, s, or w." << std::endl;
            continue;
        }
        return m_valley->movePenguins(type, dir);
    }
}

void Game::play() {
    m_valley->display("");
    Player* player = m_valley->player();
    if (player == nullptr)
        return;
    
    while ( ! player->isDead()  &&  m_valley->penguinCount() > 0) {
        std::string msg = takePlayerTurn();
        m_valley->display(msg);
        if (player->isDead())
            break;
        msg = takePenguinsTurn();
        m_valley->display(msg);
    }
    
    if (player->isDead())
        std::cout << "You lose." << std::endl;
    else
        std::cout << "You win." << std::endl;
}

bool Game::draw(sf::RenderWindow& window) {
    // -------------------
    // Set Up Penguin Game
    // -------------------
    
    gameOver = false;
    enum TurnType {Move, ThrowFish};
    TurnType turnType = TurnType::Move;
    
    sf::Font font("assets/fonts/Roboto-Regular.ttf");
    const float windowWidth = window.getSize().x;
    const float fontSize = 20.f;
    
    sf::Text title = createText(font, "Penguin Palooza", 80.f, windowWidth / 2.f, 50);
    
    sf::RectangleShape menuButton = createButton({300.f, 60.f},
                                                 {windowWidth / 2.f - 150.f, 110.f}, 3.f);
    sf::Text menuText = createText(font, "Return to main menu", fontSize, windowWidth / 2.f, 140.f);
    
    sf::Text penguinCount = createText(font, "There are 50 penguins left.", fontSize, windowWidth / 2.f, 210.f);
    
    sf::Text threwFishText = createText(font, "", fontSize, windowWidth / 2.f, 825.f);
    
    sf::Text infoText = createText(font, "Your move (WASD to move, X to do nothing, anything else for computer recommended move)", fontSize, windowWidth / 2.f, 875.f);
    
    const float boardX = 600.f;
    const float boardY = 500.f;
    const float cellSize = 50.f;
    
    const float startX = window.getSize().x / 2.f - boardX / 2.f;
    const float startY = window.getSize().y / 2.f - boardY / 2.f;
    
    // Panel for displaying throw fish move
    float panelX = startX;
    float panelY = startY + 11 * cellSize;
    
    Player* player = m_valley->player();
    Penguin* const* penguins = m_valley->penguins();
    int rows = m_valley->rows();
    int cols = m_valley->cols();
    char move = 'r';
    
    const std::string direction[4] = {"Up", "Down", "Left", "Right"};

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
                
                if (!gameOver && turnType == TurnType::ThrowFish) {
                    char directionCode[4] = {'n', 's', 'w', 'e'};
                    char speciesCode[3] = {'K', 'G', 'M'};
                    
                    if (!gameOver && mouseX >= panelX && mouseX < panelX + 12 * cellSize &&
                        mouseY >= panelY && mouseY < panelY + 3 * cellSize) {
                        
                        int row = (mouseY - panelY) / cellSize;
                        int col = (mouseX - panelX) / (3 * cellSize);
                        
                        int dir = decodeDirection(directionCode[col]);
                        m_valley->movePenguins(speciesCode[row], dir);
                        
                        threwFishText.setString("You threw a " + std::string(1, speciesCode[row]) + " fish " + direction[col]);
                        penguinMoveSound.play();
                    }
                    
                    if (player->isDead()) {
                        defeatSound.play();
                        gameOver = true;
                    } else if (m_valley->penguinCount() == 0) {
                        victorySound.play();
                        gameOver = true;
                    }
                    
                    turnType = TurnType::Move;
                }
            }
            
            if (!gameOver && turnType == TurnType::Move) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (!gameOver && keyPressed->scancode == sf::Keyboard::Scancode::W)
                        move = 'n';
                    else if (!gameOver && keyPressed->scancode == sf::Keyboard::Scancode::A)
                        move = 'w';
                    else if (!gameOver && keyPressed->scancode == sf::Keyboard::Scancode::S)
                        move = 's';
                    else if (!gameOver && keyPressed->scancode == sf::Keyboard::Scancode::D)
                        move = 'e';
                    else if (!gameOver && keyPressed->scancode == sf::Keyboard::Scancode::X)
                        move = 'x';
                    else if (!gameOver)
                        move = 'r';
                    
                    int dir = decodeDirection(move);
                    penguinMoveSound.play();
                    
                    if (move == 'r') {
                        if (recommendMove(*m_valley, player->row(), player->col(), dir))
                            player->move(dir);
                    } else if (move != 'x') {
                        player->move(dir);
                        if (player->isDead()) {
                            defeatSound.play();
                            gameOver = true;
                        }
                    }
                    
                    turnType = TurnType::ThrowFish;
                }
            }
        }
        
        window.clear(sf::Color(200, 200, 200));
        
        // -------------------
        // Render Game Objects
        // -------------------
        penguinCount.setString("There are " + std::to_string(m_valley->penguinCount()) +  " penguin(s) left.");
        recenterText(penguinCount);
        
        for (int i = 0; i < 13; i++) {
            sf::RectangleShape line({5.f, boardY + 5.f});
            line.setPosition({startX + i * cellSize, startY});
            line.setFillColor(sf::Color::Black);
            window.draw(line);
        }
        
        for (int i = 0; i < 11; i++) {
            sf::RectangleShape line({boardX, 5.f});
            line.setPosition({startX, startY + i * cellSize});
            line.setFillColor(sf::Color::Black);
            window.draw(line);
        }
        
        std::vector<std::vector<bool>> occupied (rows, std::vector<bool>(cols, false));
        
        for (int row = 1; row <= m_valley->rows(); row++) {
            for (int col = 1; col <= m_valley->cols(); col++) {
                if (m_valley->hasProtrusionAt(row, col)) {
                    float height = 20.f;
                    float side = 2.f / std::sqrt(3.f) * height;
                    sf::ConvexShape protrusion(3);
                    
                    protrusion.setPoint(0, {side / 2.f, 0.f});
                    protrusion.setPoint(1, {0.f, height});
                    protrusion.setPoint(2, {side, height});
                    
                    protrusion.setFillColor(sf::Color(139, 69, 19)); // Brown
                    protrusion.setOrigin({side / 2.f, height / 2.f});
                    protrusion.setPosition({startX + cellSize / 2 + (col - 1) * cellSize,
                                            startY + cellSize / 2 + (row - 1) * cellSize});
                    
                    window.draw(protrusion);
                } else if (row == player->row() && col == player->col()) {
                    sf::CircleShape alivePlayer(10.f);
                    alivePlayer.setFillColor(sf::Color::Blue);
                    alivePlayer.setOrigin({10.f, 10.f});
                    alivePlayer.setPosition({startX + cellSize / 2 + (col - 1) * cellSize,
                                             startY + cellSize / 2 + (row - 1) * cellSize});
                    
                    sf::Text deadPlayer = createText(font, "X", 20.f,
                                                     startX + cellSize / 2 + (col - 1) * cellSize,
                                                     startY + cellSize / 2 + (row - 1) * cellSize);
                    
                    if (player->isDead()) {
                        occupied[player->row() - 1][player->col() - 1] = true;
                        window.draw(deadPlayer);
                    } else
                        window.draw(alivePlayer);
                }
            }
        }
        
        for (int i = 0; i < m_valley->penguinCount(); i++) {
            std::string penguinSpecies = std::string(1, penguins[i]->species());
            int row = penguins[i]->row();
            int col = penguins[i]->col();
            
            sf::Text penguin = createText(font, penguinSpecies, 20.f,
                                         startX + cellSize / 2 + (col - 1) * cellSize,
                                         startY + cellSize / 2 + (row - 1) * cellSize);
            
            if (penguins[i]->species() == 'K')
                penguin.setFillColor(sf::Color(0, 100, 0)); // Green
            else if (penguins[i]->species() == 'M')
                penguin.setFillColor(sf::Color(200, 100, 0)); // Orange
            else if (penguins[i]->species() == 'G')
                penguin.setFillColor(sf::Color(128, 0, 128)); // Purple
            
            
            if (!occupied[row - 1][col - 1]) {
                window.draw(penguin);
                occupied[row - 1][col - 1] = true;
            }
        }
        
        
        if (!gameOver && turnType == TurnType::Move) {
            infoText.setString("Your move (WASD to move, X to do nothing, anything else for computer recommended move)");
        } else if (!gameOver && turnType == TurnType::ThrowFish) {
            infoText.setString("");
            threwFishText.setString("");
            
            std::string directions[4] = {"Up", "Down", "Left", "Right"};
            std::string species[3] = {"K", "G", "M"};
            
            for (int i = 0; i < 5; i++) {
                sf::RectangleShape line({5.f, 3 * cellSize + 5.f});
                line.setPosition({panelX + i * 3 * cellSize, panelY});
                line.setFillColor(sf::Color::Black);
                window.draw(line);
            }
            
            for (int i = 0; i < 4; i++) {
                sf::RectangleShape line({12 * cellSize, 5.f});
                line.setPosition({panelX, panelY + i * cellSize});
                line.setFillColor(sf::Color::Black);
                window.draw(line);
            }
            
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 4; j++) {
                    sf::Text text = createText(font, "Throw " + species[i] + " fish " + directions[j], 15.f,
                                               panelX + j * 3 * cellSize + 3 * cellSize / 2.f,
                                               panelY + i * cellSize + cellSize / 2.f);
                    window.draw(text);
                }
            }
        } else if (gameOver && m_valley->penguinCount() > 0) {
            infoText.setString("Player walked into a penguin and died.");
        } else if (gameOver && m_valley->penguinCount() == 0) {
            infoText.setString("You won. All penguins are defeated.");
        }
        
        recenterText(infoText);
        recenterText(threwFishText);
        
        window.draw(title);
        window.draw(menuText);
        window.draw(penguinCount);
        window.draw(menuButton);
        window.draw(threwFishText);
        window.draw(infoText);
        
        window.display();
    }
    
    return true;
}
