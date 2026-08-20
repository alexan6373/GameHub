#ifndef PENGUIN_H
#define PENGUIN_H

class Valley;

class Penguin
{
  public:
    // Constructor
    Penguin(Valley* vp, int r, int c, char species);

    // Accessors
    int  row() const;
    int  col() const;
    char species() const;
    bool isDead() const;

    // Mutators
    void forceMove(int dir);
    void move();

  private:
    Valley* m_valley;
    int     m_row;
    int     m_col;
    char    m_species;
    int     m_health;
};

#endif
