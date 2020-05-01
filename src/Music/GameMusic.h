#pragma once
#include <SFML/Audio.hpp>

class GameMusic {
	static sf::Music currentTrack;
public:
	static float masterVolume;
	static float musicVolume;

	static void updateVolumen() {
		currentTrack.setVolume(musicVolume / 12.5 * masterVolume / 100);
	}

	static void setVolume(int volume) {
		musicVolume = volume;
		updateVolumen();
	}

	static int getVolume() {
		return musicVolume;
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
		if (!currentTrack.openFromFile("../music/Music/Title.flac"))
			throw ExceptionLoadImage("No se ha podido cargar el track: ../music/Title.flac");
		currentTrack.play();
		updateVolumen();
	}
	
	static void playWorld1Music() {
		if (!currentTrack.openFromFile("../music/Music/World 1.flac"))
			throw ExceptionLoadImage("No se ha podido cargar el track: ../music/World 1.flac");
		currentTrack.play();
		updateVolumen();
	}
};

sf::Music GameMusic::currentTrack;
float GameMusic::masterVolume = 50;
float GameMusic::musicVolume = 50;