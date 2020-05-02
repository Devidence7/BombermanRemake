#pragma once
#include <SFML/Audio.hpp>

class GameMusic {
	static sf::Music currentTrack;
public:
	static float masterVolume;
	static float musicVolume;


private:
	static void insertMusicTrack(string musicLocation) {
		if (!currentTrack.openFromFile(musicLocation))
			throw ExceptionLoadImage("No se ha podido cargar el track: " + musicLocation);
		currentTrack.play();
		updateVolumen();
	}

public:
	static void updateVolumen() {
		currentTrack.setVolume(musicVolume / 12.5 * masterVolume / 100);
	}

	static void setVolume(int volume) {
		musicVolume = volume;
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

	static int getVolume() {
		return musicVolume;
	}

	static void playTitleMusic() {
		insertMusicTrack("../music/Music/Title.flac");
	}
	
	static void playWorld1Music() {
		insertMusicTrack("../music/Music/World 1.flac");
	}

	static void playWorld2Music() {
		insertMusicTrack("../music/Music/World 2.flac");
	}

	static void playWorld3Music() {
		insertMusicTrack("../music/Music/World 3.flac");
	}
};

sf::Music GameMusic::currentTrack;
float GameMusic::masterVolume = 50;
float GameMusic::musicVolume = 50;