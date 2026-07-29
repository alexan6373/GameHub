#ifndef PLAYER_H
#define PLAYER_H
#include <string>

class Valley;

class Player {
  public:
      // Constructor
    Player(Valley* vp, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string stand();
    std::string move(int dir);
    void   setDead();

  private:
    Valley* m_valley;
    int     m_row;
    int     m_col;
    bool    m_dead;
};

#endif
