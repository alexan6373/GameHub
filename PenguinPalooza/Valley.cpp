#include "Valley.hpp"
#include "Penguin.hpp"
#include "Player.hpp"
#include "PenguinGameHelper.hpp"

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>

///////////////////////////////////////////////////////////////////////////
//  Valley implementation
///////////////////////////////////////////////////////////////////////////

Valley::Valley(int nRows, int nCols) {
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS) {
        std::cout << "***** Valley created with invalid size " << nRows << " by "
             << nCols << "!" << std::endl;
        std::exit(1);
    }
    
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nPenguins = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_protrusionGrid[r-1][c-1] = false;
}

Valley::~Valley() {
    delete m_player; // Deallocate player
    for (int i = 0; i < m_nPenguins; i++) {
        delete m_penguins[i]; // Deallocate penguins
    }
}

int Valley::rows() const {
    return m_rows;
}

int Valley::cols() const {
    return m_cols;
}

Player* Valley::player() const {
    return m_player;
}

Penguin* const* Valley::penguins() const {
    return m_penguins;
}

int Valley::penguinCount() const {
    return m_nPenguins;
}

bool Valley::hasProtrusionAt(int r, int c) const {
    checkPos(r, c, "Valley::hasProtrusionAt");
    return m_protrusionGrid[r-1][c-1];
}

int Valley::numberOfPenguinsAt(int r, int c) const {
    int count = 0;
    
    for (int i = 0; i < m_nPenguins; i++) {
        if ((m_penguins[i]->row() == r) && (m_penguins[i]->col() == c)) {
            count++;
        }
    }
    
    return count;
}

void Valley::display(std::string msg) const {
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
//    Fill displayGrid with dots (empty) and stars (protrusion)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (hasProtrusionAt(r, c) ? '*' : '.');

//    Indicate penguin positions by their species.  If more than one penguin
//    occupies a spot, show just one (any one will do).
    for (int i = 0; i < m_nPenguins; i++) {
        displayGrid[m_penguins[i]->row() - 1][m_penguins[i]->col() - 1] = m_penguins[i]->species();
    }

//   Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

//   Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++) {
        for (c = 1; c <= cols(); c++)
            std::cout << displayGrid[r-1][c-1];
        std::cout << std::endl;
    }
    
    std::cout << std::endl;

//   Write message, penguin, and player info
    if (msg != "")
        std::cout << msg << std::endl;
    std::cout << "There are " << penguinCount() << " penguins remaining." << std::endl;
    
    if (m_player == nullptr)
        std::cout << "There is no player!" << std::endl;
    else if (m_player->isDead())
        std::cout << "The player is dead." << std::endl;
}

void Valley::placeProtrusionAt(int r, int c) {
    checkPos(r, c, "Valley::placeProtrusionAt");
    m_protrusionGrid[r-1][c-1] = true;
}

bool Valley::addPenguin(int r, int c, char species) {
    if (! isPosInBounds(r, c)  ||  hasProtrusionAt(r, c))
        return false;
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    if (species != 'G'  &&  species != 'M'  &&  species != 'K')
        return false;
    if (m_nPenguins == MAXPENGUINS)
        return false;
    m_penguins[m_nPenguins] = new Penguin(this, r, c, species);
    m_nPenguins++;
    return true;
}

bool Valley::addPlayer(int r, int c) {
    if (m_player != nullptr  ||  ! isPosInBounds(r, c)  ||  hasProtrusionAt(r, c))
        return false;
    if (numberOfPenguinsAt(r, c) > 0)
        return false;
    m_player = new Player(this, r, c);
    return true;
}

std::string Valley::movePenguins(char species, int dir) {
//   Penguins belonging to the species will respond with probability 1/2
    bool willRespond = (randInt(0, 1) == 0);

//   Move all penguins
    int nPenguinsOriginally = m_nPenguins;
    
    for (int i = 0; i < m_nPenguins; i++) {
        if (m_penguins[i]->species() == species && willRespond) {
            m_penguins[i]->forceMove(dir);
        } else {
            m_penguins[i]->move();
        }
    }
    
// Check if player is dead
    for (int i = 0; i < m_nPenguins; i++) {
        if(m_penguins[i]->row() == m_player->row() && m_penguins[i]->col() == m_player->col()) {
            m_player->setDead();
        }
    }
    
// Deallocate dead penguins
    for (int i = 0; i < m_nPenguins; i++) {
        if (m_penguins[i]->isDead()) {
            delete m_penguins[i];
            for (int j = i; j < m_nPenguins - 1; j++) {
                m_penguins[j] = m_penguins[j + 1];
            }
            m_nPenguins--;
        }
    }
    
    if (m_nPenguins < nPenguinsOriginally)
        return "Some penguins have been destroyed.";
    else
        return "No penguins were destroyed.";
}

bool Valley::isPosInBounds(int r, int c) const {
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Valley::checkPos(int r, int c, std::string functionName) const {
    if (!isPosInBounds(r, c)) {
        std::cout << "***** " << "Invalid valley position (" << r << ","
             << c << ") in call to " << functionName << std::endl;
        std::exit(1);
    }
}
