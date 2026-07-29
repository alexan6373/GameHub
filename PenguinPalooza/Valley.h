#ifndef VALLEY_H
#define VALLEY_H

#include "Helper.h"
#include <string>

class Player;
class Penguin;

class Valley {
  public:
  // Constructor/destructor
    Valley(int nRows, int nCols);
    ~Valley();

  // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     penguinCount() const;
    bool    hasProtrusionAt(int r, int c) const;
    int     numberOfPenguinsAt(int r, int c) const;
    void    display(std::string msg) const;

  // Mutators
    void   placeProtrusionAt(int r, int c);
    bool   addPenguin(int r, int c, char species);
    bool   addPlayer(int r, int c);
    std::string movePenguins(char species, int dir);

  private:
    bool     m_protrusionGrid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Penguin* m_penguins[MAXPENGUINS];
    int      m_nPenguins;

  // Helper functions
    void checkPos(int r, int c, std::string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

#endif
