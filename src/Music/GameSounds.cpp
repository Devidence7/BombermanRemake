#include "GameSounds.h"

float GameSounds::soundVolume = 50.0;
unsigned int GameSounds::currentSound = 0;

sf::SoundBuffer GameSounds::soundBuffer[MAX_SOUND];
std::deque<sf::Sound> GameSounds::soundInstances;