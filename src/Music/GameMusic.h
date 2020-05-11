#pragma once
#include <SFML/Audio.hpp>
#include "../Exceptions/ExceptionsGame.hpp"

class GameMusic {
	static sf::Music currentTrack;
	static float musicVolume;
	static float masterVolume;

	static void insertMusicTrack(std::string musicLocation) {
		if (!currentTrack.openFromFile(musicLocation))
			throw ExceptionLoadImage("No se ha podido cargar el track: " + musicLocation);
		currentTrack.play();
		currentTrack.setLoop(true);
		updateVolumen();
	}

public:
	static void updateVolumen() {
		currentTrack.setVolume(musicVolume / 12.5 * masterVolume / 100);
	}


	static int getVolume() {
		return musicVolume;
	}

	static void setVolume(int volume) {
		musicVolume = volume;
		updateVolumen();
	}

	static int getMasterVolume() {
		return masterVolume;
	}

	static void setMasterVolume(int volume) {
		masterVolume = volume;
		updateVolumen();
	}


	static void volumeUp() {
		musicVolume = musicVolume + 1;
		updateVolumen();
	}

	static void volumeDown() {
		musicVolume = musicVolume - 1;
		updateVolumen();
	}


	static void playTitleMusic() {
		insertMusicTrack("../music/Music/Title.flac");
	}
	
	static void playWorld1Music() {
		insertMusicTrack("../music/Music/World 1.flac");
	}

	static void playWorld2Music() {
		insertMusicTrack("../music/Music/World 2.wav");
	}

	static void playWorld3Music() {
		insertMusicTrack("../music/Music/World 3.wav");
	}

};