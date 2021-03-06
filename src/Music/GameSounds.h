#pragma once
#include <SFML/Audio.hpp>
#include "GameMusic.h"
#include "../Exceptions/ExceptionsGame.hpp"
#include "../Logic/Random.h"
#include <deque>

class GameSounds {
	static float soundVolume;

	static const int MAX_SOUND = 128;

	static unsigned int currentSound;

	static std::deque<sf::Sound> soundInstances;
	static sf::SoundBuffer soundBuffer[MAX_SOUND];

	// TODO: MUTEX REQUIRED
	static unsigned int getCurrentIndice() {
		currentSound++;
		if (!currentSound < MAX_SOUND) {
			currentSound = 0;
		}
		int returnVal = currentSound;

		return returnVal;
	}

	static void insertSoundTrack(std::string soundLocation, int track) {
		if (!soundBuffer[track].loadFromFile(soundLocation)) {
			// throw ExceptionLoadImage("No se ha podido cargar el track: " + soundLocation);
		}
	}

	static void playTrack(int track) {
		unsigned int currentIndice = getCurrentIndice();

		soundInstances.push_back(sf::Sound(soundBuffer[track]));
		soundInstances.back().setVolume(2 * soundVolume / 12.5 * GameMusic::getMasterVolume() / 100);
		soundInstances.back().play();
	}

public:

	static void inicializeSounds() {
		insertSoundTrack("../music/Sounds/Bombs/Sound Effects (185).wav", 0);
		insertSoundTrack("../music/Sounds/Bombs/Sound Effects (104).wav", 1);
		insertSoundTrack("../music/Sounds/Bombs/Battle Mode Sounds (41).wav", 2);
		insertSoundTrack("../music/Sounds/Bombs/Sound Effects (141).wav", 3);

		insertSoundTrack("../music/Sounds/Enemies/Dead/Monster Sounds (70).wav", 4);
		insertSoundTrack("../music/Sounds/Enemies/Dead/Monster Sounds (71).wav", 5);

		insertSoundTrack("../music/Sounds/Teleporter/Sound Effects (283).wav", 6);
		insertSoundTrack("../music/Sounds/Teleporter/Sound Effects (142).wav", 7);
		insertSoundTrack("../music/Sounds/Teleporter/Arrive Teleporter.wav", 8);

		insertSoundTrack("../music/Sounds/Menu/Button Press.wav", 9);
		insertSoundTrack("../music/Sounds/Menu/Game Error.wav", 10);
		insertSoundTrack("../music/Sounds/Menu/Sound Change.wav", 11);
		insertSoundTrack("../music/Sounds/Menu/The_Game_Over.wav", 12);

		insertSoundTrack("../music/Sounds/Player/Player Dead.wav", 13);
	}

	static void Update(void) {
		for (int i = 0; i < soundInstances.size(); ++i) {
			if (soundInstances[i].getStatus() == sf::Sound::Stopped) {
				soundInstances.erase(soundInstances.begin() + i);
				--i;
			}
		}
	}

	static unsigned int Count() {
		soundInstances.size();
	}


	static int getVolume() {
		return soundVolume;
	}

	static void setVolume(int volume) {
		soundVolume = volume;
	}

	static void playBombSound() {
		switch (Random::getIntNumberBetween(0, 3)) {
		case 1:
			playTrack(0);
			break;
		case 2:
			playTrack(1);
			break;
		case 3:
			playTrack(2);
			break;
		default:
			playTrack(3);
			break;
		} 
	}

	static void playEnemyDied() {
		switch (Random::getIntNumberBetween(0, 1)) {
		case 0:
			playTrack(4);
			break;
		case 1:
			playTrack(5);
			break;
		}
	}

	static void teleportAppear() {
		playTrack(6);
	}

	static void teleportEnter() {
		playTrack(7);
	}

	static void buttonPress() {
		playTrack(9);
	}
	static void buttonError() {
		playTrack(10);
	}
	static void soundChange() {
		playTrack(11);
	}
	static void gameOverSound() {
		playTrack(12);
	}

	static void playerDead() {
		playTrack(13);
	}
};

