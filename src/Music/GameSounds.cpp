#include "GameSounds.h"

float GameSounds::soundVolume = 50.0;
unsigned int GameSounds::currentSound = 0;

sf::Sound GameSounds::sound[MAX_SOUND];
sf::SoundBuffer GameSounds::soundBuffer;