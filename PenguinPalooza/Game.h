#ifndef GAME_H
#define GAME_H
#include <string>

class Valley;

class Game {
  public:
  // Constructor/destructor
    Game(int rows, int cols, int nPenguins);
    ~Game();

  // Mutators
    void play();

  private:
    Valley* m_valley;

  // Helper functions
    std::string takePlayerTurn();
    std::string takePenguinsTurn();
};

#endif
