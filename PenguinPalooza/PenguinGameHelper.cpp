#include "PenguinGameHelper.hpp"
#include "Valley.hpp"

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>

int decodeDirection(char dir) {
    switch (dir) {
      case 'n':  return NORTH;
      case 'e':  return EAST;
      case 's':  return SOUTH;
      case 'w':  return WEST;
    }
    return BADDIR;  // bad argument passed in!
}

//    Return a random int from min to max, inclusive
int randInt(int min, int max) {
    if (max < min)
        std::swap(max, min);
    static std::random_device rndm;
    static std::default_random_engine generator(rndm());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

//    Return false without changing anything if moving one step from (r,c)
//    in the indicated direction would hit a protrusion or run off the edge of
//    the valley.  Otherwise, update r and c to the position resulting from the
//    move and return true.
bool attemptMove(const Valley& v, int dir, int& r, int& c) {
    
    if (dir == 0) { // Command to move north
        if (r == 1 || v.hasProtrusionAt(r - 1, c)) { // check for cliff wall or protrusion
            return false;
        } else {
            r--;
            return true;
        }
        
    } else if (dir == 1) { // Command to move east
        if (c == v.cols() || v.hasProtrusionAt(r, c + 1)) { // check for cliff wall or protrusion
            return false;
        } else {
            c++;
            return true;
        }
        
    } else if (dir == 2) { // Command to move south
        if (r == v.rows() || v.hasProtrusionAt(r + 1, c)) { // check for cliff wall or protrusion
            return false;
        } else {
            r++;
            return true;
        }
        
    } else if (dir == 3) { // Command to move west
        if (c == 1 || v.hasProtrusionAt(r, c - 1)) { // check for cliff wall or protrusion
            return false;
        } else {
            c--;
            return true;
        }
    }
    
    return false;
}

//    Recommend a move for a player at (r,c):  A false return means the
//    recommendation is that the player should stand; otherwise, bestDir is
//    set to the recommended direction to move.

// -----------------------------------------------------------------------------------------------
// ALGORITHM
// 1. Calculate how many adjacent penguins that could randomly walk into the player
//    for the 5 moves: stand, move north, move east, move south, move west
// 2. Whichever move results in being adjacent to the least amount of penguins is chosen
// 3. In case of a tie, move precendence goes stand, move north, move east, move south, move west
// 4. The algorithm also prevents the player from walking into a penguin and losing
// -----------------------------------------------------------------------------------------------

bool recommendMove(const Valley& v, int r, int c, int& bestDir) {
    int minPenguins = v.numberOfPenguinsAt(r - 1, c) + v.numberOfPenguinsAt(r + 1, c) +
                      v.numberOfPenguinsAt(r, c - 1) + v.numberOfPenguinsAt(r, c + 1);
    char bestMove = 'x';
    int numPenguins = minPenguins;
        
    if (r != 1 && !v.hasProtrusionAt(r - 1, c) && v.numberOfPenguinsAt(r - 1, c) == 0) {
        numPenguins = v.numberOfPenguinsAt(r - 2, c) + v.numberOfPenguinsAt(r - 1, c - 1) +
                          v.numberOfPenguinsAt(r - 1, c + 1);
        if (numPenguins < minPenguins) {
            minPenguins = numPenguins;
            bestMove = 'n';
        }
    }
   
        
    if (c != v.cols() && !v.hasProtrusionAt(r, c + 1) && v.numberOfPenguinsAt(r, c + 1) == 0) {
        numPenguins = v.numberOfPenguinsAt(r - 1, c + 1) + v.numberOfPenguinsAt(r + 1, c + 1) +
                          v.numberOfPenguinsAt(r, c + 2);
        if (numPenguins < minPenguins) {
            minPenguins = numPenguins;
            bestMove = 'e';
        }
    }
        
    if (r != v.rows() && !v.hasProtrusionAt(r + 1, c) && v.numberOfPenguinsAt(r + 1, c) == 0) {
        numPenguins = v.numberOfPenguinsAt(r + 2, c) + v.numberOfPenguinsAt(r + 1, c - 1) +
                          v.numberOfPenguinsAt(r + 1, c + 1);
        if (numPenguins < minPenguins) {
            minPenguins = numPenguins;
            bestMove = 's';
        }
    }
    
    if (c != 1 && !v.hasProtrusionAt(r, c - 1) && v.numberOfPenguinsAt(r, c - 1) == 0) {
        numPenguins = v.numberOfPenguinsAt(r - 1, c - 1) + v.numberOfPenguinsAt(r + 1, c - 1) +
                          v.numberOfPenguinsAt(r, c - 2);
        if (numPenguins < minPenguins) {
            minPenguins = numPenguins;
            bestMove = 'w';
        }
    }
    
    if (bestMove == 'n') {
        bestDir = 0;
        return true;
    } else if (bestMove == 'e') {
        bestDir = 1;
        return true;
    } else if (bestMove == 's') {
        bestDir = 2;
        return true;
    } else if (bestMove == 'w') {
        bestDir = 3;
        return true;
    }
    
    return false;
                    
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen() { // will just write a newline in an Xcode output window

    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        std::cout << std::endl;
    else {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        std::cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << std::flush;
    }
}

#endif
