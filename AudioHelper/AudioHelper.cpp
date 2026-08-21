#include "AudioHelper.hpp"

sf::SoundBuffer clickBuffer("assets/sounds/click1.ogg");
sf::Sound clickSound(clickBuffer);

sf::SoundBuffer fifteenMoveBuffer("assets/sounds/rollover6.ogg");
sf::Sound fifteenMoveSound(fifteenMoveBuffer);

sf::SoundBuffer hangmanMoveBuffer("assets/sounds/rollover2.ogg");
sf::Sound hangmanMoveSound(hangmanMoveBuffer);

sf::SoundBuffer penguinMoveBuffer("assets/sounds/footstep02.ogg");
sf::Sound penguinMoveSound(penguinMoveBuffer);

sf::SoundBuffer tictactoeMoveBuffer("assets/sounds/switch1.ogg");
sf::Sound tictactoeSound(tictactoeMoveBuffer);

sf::SoundBuffer victorySoundBuffer("assets/sounds/jingles_NES03.ogg");
sf::Sound victorySound(victorySoundBuffer);

sf::SoundBuffer defeatSoundBuffer("assets/sounds/jingles_NES00.ogg");
sf::Sound defeatSound(defeatSoundBuffer);
