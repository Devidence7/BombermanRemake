#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
class GameDisplayController {
	sf::RenderWindow* window;

public:
	unsigned int FPSs;
	unsigned int windowWidth;
	unsigned int windowHeight;


    enum GameState {
        MAIN_MENU,
        PLAYING,
        OPTIONS_MENU,
        PAUSE_MENU
    };

    GameState gameState;
    bool screenChanged;

	GameDisplayController() {
		// Get display properties from properties file
		windowWidth = sf::VideoMode::getDesktopMode().width;
		windowHeight = sf::VideoMode::getDesktopMode().height;
		FPSs = 60;

		// Start in MAIN_MENU
		gameState = GameState::MAIN_MENU;

		// Create the main window
		window = new sf::RenderWindow(sf::VideoMode(27 * 48, 17 * 48), "Bombermenaman"); // , sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen
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

    void manageGameInterface(std::function<void(sf::Event&)> buttonActions) {
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
				//createBackgroundMenu(*window, false);
				break;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//													BUTTON PRESSED
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
				buttonActions(event);
				break;
			}
		}
    }
};