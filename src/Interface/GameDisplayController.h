#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "../Music/GameSounds.h"
#include <fstream>
#include "../Utils/PropertiesReader.h"

class GameDisplayController {
	sf::RenderWindow* window;

public:
	unsigned int FPSs;
	unsigned int windowWidth;
	unsigned int windowHeight;
	bool fullScreen;

    enum GameState {
        MAIN_MENU,
        PLAYING,
        OPTIONS_MENU,
        PAUSE_MENU
    };

    GameState gameState;

	PropReader::data gameProperties;
    
	bool mainMenuReprocessDisplay = false;
	bool playingReprocessDisplay = false;
	bool optionsMenuReprocessDisplay = false;
	bool pauseMenuReprocessDisplay = false;

	GameDisplayController() {
		// Get display properties from properties file
		getProperties();

		// Start in MAIN_MENU
		gameState = GameState::MAIN_MENU;

		// Create the main window
		if (fullScreen) {
			window = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Bombermenaman", sf::Style::Fullscreen); // , sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen
		}
		else {
			window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Bombermenaman");
		}
		
		window->setFramerateLimit(FPSs);
	}

	bool windowOpen() {
		return window->isOpen();
	}

	sf::RenderWindow* &getWindow() {
		return window;
	}
	
	GameState getGameState() {
		return gameState;
	}
	
	void setGameState(GameState newGameState) {
		gameState = newGameState;
	}

	void notifyChangeDisplay() {
		mainMenuReprocessDisplay = true;
		playingReprocessDisplay = true;
		optionsMenuReprocessDisplay = true;
		pauseMenuReprocessDisplay = true;
	}

    void manageGameInterface(GameDisplayController &gameDisplay, std::function<void(sf::Event&)> buttonActions = nullptr) {
		sf::Event event;
		while (window->pollEvent(event)) {
			// Process events
			switch (event.type) {
				// window closed
			case sf::Event::Closed:
				// Close window -> exit
				window->close();
				break;
			case sf::Event::LostFocus:
				// Pause
				break;
			case sf::Event::GainedFocus:
				// Resume
				break;
			case sf::Event::Resized: {
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window->setView(sf::View(visibleArea));
				window->setSize(sf::Vector2u(event.size.width, event.size.height));
				gameDisplay.windowWidth = event.size.width;
				gameDisplay.windowHeight = event.size.height;
				gameDisplay.saveProperties();
				notifyChangeDisplay();
				break;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//									   IF MENU BUTTON PRESSED OR OTHE CUSTOM
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
				if (buttonActions) {
					buttonActions(event);
				}
				break;
			}
		}
    }


	void getProperties() {
		string filename = "properties.txt";
		fstream appendFileToWorkWith;

		appendFileToWorkWith.open(filename, std::fstream::in);

		// If file does not exist, Create new file
		if (!appendFileToWorkWith.is_open()) {
			appendFileToWorkWith.close();
			appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | fstream::trunc);
			appendFileToWorkWith << "volume.master = 50\n";
			appendFileToWorkWith << "volume.music = 50\n";
			appendFileToWorkWith << "volume.sound = 50\n\n";
			appendFileToWorkWith << "resolution.x = 1000\n";
			appendFileToWorkWith << "resolution.y = 500\n";
			appendFileToWorkWith << "fullscreen = 0\n";
			appendFileToWorkWith << "fps = 60\n";

			appendFileToWorkWith.close();

		}
		else {
			appendFileToWorkWith.close();
		}

		appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out);

		appendFileToWorkWith >> gameProperties;
		PropReader::data::const_iterator iter;

		for (iter = gameProperties.begin(); iter != gameProperties.end(); iter++) {
			cout << iter->first << " = " << iter->second << endl;
		}

		cout << gameProperties.at("volume.master") << endl;
		GameMusic::setMasterVolume(stoi(gameProperties.at("volume.master")));
		GameMusic::setVolume(stoi(gameProperties.at("volume.music")));
		GameSounds::setVolume(stoi(gameProperties.at("volume.sound")));

		FPSs = stoi(gameProperties.at("fps"));
		fullScreen = (bool)stoi(gameProperties.at("fullscreen"));
		windowWidth = stoi(gameProperties.at("resolution.x"));
		windowHeight = stoi(gameProperties.at("resolution.y"));

		appendFileToWorkWith.close();
	}

	void saveProperties() {
		gameProperties.at("volume.master") = to_string(GameMusic::getMasterVolume());
		gameProperties.at("volume.music") = to_string(GameMusic::getVolume()); 
		gameProperties.at("volume.sound") = to_string(GameMusic::getVolume());

		gameProperties.at("fps") = to_string(FPSs);
		gameProperties.at("fullscreen") = to_string((int)fullScreen);
		gameProperties.at("resolution.x") = to_string(windowWidth);
		gameProperties.at("resolution.y") = to_string(windowHeight);

		string filename = "properties.txt";
		fstream file;
		file.open(filename, std::fstream::in | std::fstream::out | fstream::trunc);
		file << gameProperties;
	}

};