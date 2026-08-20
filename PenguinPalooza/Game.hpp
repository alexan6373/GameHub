#ifndef GAME_H
#define GAME_H
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Valley;

class Game {
  public:
  // Constructor/destructor
    Game(int rows, int cols, int nPenguins);
    ~Game();

  // Mutators
    void play();
    bool drawPenguinGame(sf::RenderWindow& window);

  private:
    Valley* m_valley;
    bool gameOver;

  // Helper functions
    std::string takePlayerTurn();
    std::string takePenguinsTurn();
};

#endif
