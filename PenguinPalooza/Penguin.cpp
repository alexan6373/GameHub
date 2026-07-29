#include "Penguin.h"
#include "Valley.h"
#include "Helper.h"

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>

///////////////////////////////////////////////////////////////////////////
//  Penguin implementation
///////////////////////////////////////////////////////////////////////////

Penguin::Penguin(Valley* vp, int r, int c, char species) {
    if (vp == nullptr) {
        std::cout << "***** A penguin must be created in some Valley!" << std::endl;
        std::exit(1);
    }
    
    if (r < 1  ||  r > vp->rows()  ||  c < 1  ||  c > vp->cols()) {
        std::cout << "***** Penguin created with invalid coordinates (" << r << ","
             << c << ")!" << std::endl;
        std::exit(1);
    }
    
    if (species != 'G'  &&  species != 'M'  &&  species != 'K') {
        std::cout << "***** Penguin created with invalid species " << species << std::endl;
        std::exit(1);
    }
    
    m_valley = vp;
    m_row = r;
    m_col = c;
    m_species = species;
    m_health = INITIAL_PENGUIN_HEALTH;
}

int Penguin::row() const {
    return m_row;
}

int Penguin::col() const {
    return m_col;
}

char Penguin::species() const {
    return m_species;
}

bool Penguin::isDead() const {
    return m_health <= 0;
}

void Penguin::forceMove(int dir) {
      // Suffer one unit of damage if moving fails.
    
    if (dir == 0) { // Command to move force north
        if (m_row == 1 || m_valley->hasProtrusionAt(m_row - 1, m_col)) { // checks for cliff walls or protrusions
            m_health--;
        } else {
            m_row--;
        }
        
    } else if (dir == 1) { // Command to force move east
        if (m_col == m_valley->cols() || m_valley->hasProtrusionAt(m_row, m_col + 1)) { // checks for cliff walls or protrusions
            m_health--;
        } else {
            m_col++;
        }
        
    } else if (dir == 2) { // Command to force move south
        if (m_row == m_valley->rows() || m_valley->hasProtrusionAt(m_row + 1, m_col)) { // checks for cliff walls or protrusions
            m_health--;
        } else {
            m_row++;
        }
        
    } else if (dir == 3) { // Command to force move west
        if (m_col == 1 || m_valley->hasProtrusionAt(m_row, m_col - 1)) { // checks for cliff walls or protrusions
            m_health--;
        } else {
            m_col--;
        }
    }
}

void Penguin::move() {
//    Attempt to move in a random direction; don't move if cannot move
    if (!isDead())
        attemptMove(*m_valley, randInt(0, NUMDIRS-1), m_row, m_col);
}
