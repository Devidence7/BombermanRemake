#pragma once
class GameInterfaceController {
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

	GameInterfaceController() {
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

    void manageGameInterface() {

    }
};