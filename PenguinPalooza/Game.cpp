#include "Game.h"
#include "Valley.h"
#include "Player.h"
#include "Helper.h"

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
