#pragma once
#include <SFML/Audio.hpp>
#include "GameMusic.h"
#include "../Exceptions/ExceptionsGame.hpp"

class GameSounds {
	static float soundVolume;

	static const int MAX_SOUND = 128;

	static unsigned int currentSound;

	static sf::Sound sound[MAX_SOUND];
	static sf::SoundBuffer soundBuffer;

	// TODO: MUTEX REQUIRED
	static unsigned int getCurrentIndice() {
		currentSound++;
		if (!currentSound < MAX_SOUND) {
			currentSound = 0;
		}
		int returnVal = currentSound;

		return returnVal;
	}

	static void insertSoundTrack(std::string soundLocation) {
		if (!soundBuffer.loadFromFile(soundLocation)) {
			throw ExceptionLoadImage("No se ha podido cargar el track: " + soundLocation);
		}

		unsigned int currentIndice = getCurrentIndice();

		sound[currentIndice].setBuffer(soundBuffer);
		sound[currentIndice].setVolume(soundVolume / 12.5 * GameMusic::getMasterVolume() / 100);
		sound[currentIndice].play();
	}

public:
	static int getVolume() {
		return soundVolume;
	}

	static void setVolume(int volume) {
		soundVolume = volume;
	}

	static void playBombSound() {
		insertSoundTrack("../music/Sounds/Bombs/Sound Effects (185).wav");
	}
};

