#include "Player.h"
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
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Valley* vp, int r, int c) {
    if (vp == nullptr) {
        std::cout << "***** The player must be created in some Valley!" << std::endl;
        std::exit(1);
    }
    
    if (r < 1  ||  r > vp->rows()  ||  c < 1  ||  c > vp->cols()) {
        std::cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << std::endl;
        std::exit(1);
    }
    
    m_valley = vp;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const {
    return m_row;
}

int Player::col() const {
    return m_col;
}

std::string Player::stand() {
    return "Player stands.";
}

std::string Player::move(int dir) {
    if (dir == 0) { // Command to move player north
        if (m_row == 1 || m_valley->hasProtrusionAt(m_row - 1, m_col)) { // checks for cliff walls or protrusions
            return "Player couldn't move; player stands.";
        } else if (m_valley->numberOfPenguinsAt(m_row - 1, m_col) > 0) { // checks if player walks into a penguin
            m_row--;
            m_dead = true;
            return "Player walked into a penguin and died.";
        } else {
            m_row--;
            return "Player moved north.";
        }
        
    } else if (dir == 1) { // Command to move player east
        if (m_col == m_valley->cols() || m_valley->hasProtrusionAt(m_row, m_col + 1)) { // checks for cliff walls or protrusions
            return "Player couldn't move; player stands.";
        } else if (m_valley->numberOfPenguinsAt(m_row, m_col + 1) > 0) { // checks if player walks into a penguin
            m_col++;
            m_dead = true;
            return "Player walked into a penguin and died.";
        } else {
            m_col++;
            return "Player moved east.";
        }
        
    } else if (dir == 2) { // Command to move player south
        if (m_row == m_valley->rows() || m_valley->hasProtrusionAt(m_row + 1, m_col)) { // checks for cliff walls or protrusions
            return "Player couldn't move; player stands.";
        } else if (m_valley->numberOfPenguinsAt(m_row + 1, m_col) > 0) { // checks if player walks into a penguin
            m_row++;
            m_dead = true;
            return "Player walked into a penguin and died.";
        } else {
            m_row++;
            return "Player moved south.";
        }
        
    } else if (dir == 3) { // Command to move player west
        if (m_col == 1 || m_valley->hasProtrusionAt(m_row, m_col - 1)) { // checks for cliff walls or protrusions
            return "Player couldn't move; player stands.";
        } else if (m_valley->numberOfPenguinsAt(m_row, m_col - 1) > 0) { // checks if player walks into a penguin
            m_col--;
            m_dead = true;
            return "Player walked into a penguin and died.";
        } else {
            m_col--;
            return "Player moved west.";
        }
    }

    return "Player couldn't move; player stands.";
}

bool Player::isDead() const {
    return m_dead;
}

void Player::setDead() {
    m_dead = true;
}
