#pragma once
#include <SFML/Audio.hpp>

class GameMusic {
	static sf::Music currentTrack;
public:
	static void setVolume(int volume) {
		currentTrack.setVolume(volume);
	}

	static int getVolume() {
		return currentTrack.getVolume();
	}

	static void volumeUp() {
		currentTrack.setVolume(currentTrack.getVolume() + 1);
	}

	static void volumeDown() {
		currentTrack.setVolume(currentTrack.getVolume() - 1);
	}

	static void playTitleMusic() {
		if (!currentTrack.openFromFile("../music/Title.flac"))
			throw ExceptionLoadImage("No se ha podido cargar el track: ../music/Title.flac");
		currentTrack.play();
	}
	
	static void playWorld1Music() {
		if (!currentTrack.openFromFile("../music/World 1.flac"))
			throw ExceptionLoadImage("No se ha podido cargar el track: ../music/World 1.flac");
		currentTrack.play();
	}
};

sf::Music GameMusic::currentTrack;