#pragma once
#include <functional>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "UserKeyPress.h"
#include "GUI/Theme.hpp"
#include "../Utils/PropertiesReader.h"
#include "../Music/GameSounds.h"
#include "InterfacePaths.h"

/**
 * User Interface master controller class
 */
class GameDisplayController {
	sf::RenderWindow* window;

public:
	sf::View menuView;
	sf::View camera;
	sf::View playingGuiView;

	unsigned int FPSs;
	unsigned int windowWidth;
	unsigned int windowHeight;
	bool fullScreen;
    const string PROPERTIES_FILENAME = "properties.txt";

	bool tutorialIntro = false;
	bool tutorialAbility = false;

    enum GameState {
        MAIN_MENU,
        PLAYING,
        OPTIONS_MENU,
        PAUSE_MENU,
		GAME_OVER,
		STORY_MENU,
		VS_NUMPLAYERS_MENU,
		DIFFICULTY,
		LOADING,
		VICTORY,
		FINAL_SCORE,
		END_BATTLE,
		PICK_COLOR,
		PICK_MAP,
		TUTORIAL
		//RESTART
    };

    GameState gameState;
	UserKeyPress* userKeyPress;

	PropReader::data gameProperties;

	// Background:
	sf::Texture textureBomberman;
	sf::Sprite backgroundBomberman;
	sf::Texture textureSquares;
	sf::Sprite backgroundSquares;

	double backgroundOpacity;
	double lastBackGroundOpacityChanged;
	sf::Color backgroundSquaresColor;

    
	bool mainMenuReprocessDisplay = false;
	bool playingReprocessDisplay = false;
	bool optionsMenuReprocessDisplay = false;
	bool pauseMenuReprocessDisplay = false;
	bool gameOverReprocessDisplay = false;
	bool victoryReprocessDisplay = false;
	bool scoreReprocessDisplay = false;
	bool multiplayerReprocessDisplay = false;
	bool storyReprocessDisplay = false;
	bool endBattleReprocessDisplay = false;
	bool gameInterfaceReprocessDisplay = false;
	bool colorPickerReprocessDisplay = false;
	bool tutorialReprocessDisplay = false;

	int tutorialType = 1;

	bool newScore = false;
	
	/**
	 * Constructor
	 */
	GameDisplayController() {
		// Set default style for menus:
		menuGUIThemeOptions();

		// Get display properties from properties file
		userKeyPress = getProperties();

		// Start in MAIN_MENU
		gameState = GameState::MAIN_MENU;

		// Create the main window
		if (fullScreen) {
			window = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Bomberman", sf::Style::Fullscreen); // , sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen
		}
		else {
			window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Bomberman");
		}
		window->setFramerateLimit(FPSs);
		

		// CAMERA
		menuView = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
		camera = sf::View (sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
		camera.setViewport(sf::FloatRect(0.f, 0.1f, 1.f, 1.f));
		playingGuiView = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y / 5));
		playingGuiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.2f));

		// Background Bomberman:
		textureBomberman.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
		backgroundBomberman.setTexture(textureBomberman);
		float resizeVal = std::fmax((float)window->getSize().x / textureBomberman.getSize().x, (float)window->getSize().y / textureBomberman.getSize().y);
		backgroundBomberman.setScale(resizeVal, resizeVal);

		// BackgroundSquares:
		textureSquares.loadFromFile("../textures/interface/Background_orange_squares.png");
		textureSquares.setRepeated(true);

		backgroundOpacity = 0;
		backgroundSquaresColor = sf::Color(255, 255, 0, 0);

		backgroundSquares.setColor(backgroundSquaresColor);
		backgroundSquares.setTexture(textureSquares);
		backgroundSquares.setScale(sf::Vector2f(2, 2));
		backgroundSquares.setTextureRect({ window->getPosition().x, window->getPosition().y, (int)window->getSize().x, (int)window->getSize().y });
	}

	sf::Sprite &getSquaresBackground() {
		lastBackGroundOpacityChanged = GameTime::getTimeNow();
		double OpacityChanged = fmin(lastBackGroundOpacityChanged, 0.1) * 20;

		backgroundOpacity = std::fmin(backgroundOpacity + OpacityChanged, 200);
		backgroundSquaresColor = sf::Color(255, 255, 0, (sf::Uint8)backgroundOpacity);
		backgroundSquares.setColor(backgroundSquaresColor);
		return backgroundSquares;
	}

	/**
	 * Set menu default typo, buttons and color
	 */
	void menuGUIThemeOptions() {
		GameGUI::Theme::loadFont("../textures/mainMenu/BOMBERMAN.ttf");
		GameGUI::Theme::loadTexture("../textures/interface/bomberman.png");
		GameGUI::Theme::textCharacterSize = 26;
		GameGUI::Theme::click.textColor = sf::Color::Black;
		GameGUI::Theme::click.textColorHover = sf::Color::Red;
		GameGUI::Theme::click.textColorFocus = sf::Color::Red;
		GameGUI::Theme::input.textColor = sf::Color::Black;
		GameGUI::Theme::input.textColorHover = sf::Color::Black;
		GameGUI::Theme::input.textColorFocus = sf::Color::Black;
		GameGUI::Theme::PADDING = 10.f;
	}

	/**
	 * Get window open state
	 * @return if game render window is open
	 */
	bool windowOpen() {
		return window->isOpen();
	}

	/**
	 * Return a reference to the pointer of the Window (to be change if fullScreen is needed)
	 * @return
	 */
	sf::RenderWindow* &getWindow() {
		return window;
	}

	/**
	 * Get actual user interface that it is been displayed
	 * @return
	 */
	GameState getGameState() {
		return gameState;
	}

	/**
	 * Set a new user interface to e displayed
	 * @param newGameState
	 */
	void setGameState(GameState newGameState) {
		gameState = newGameState;
	}

	/**
	 * Notify interfaces if a display option has been change (to recompute geometry, etc ...)
	 */
	void notifyChangeDisplay() {
		float resizeVal = std::fmax((float)window->getSize().x / textureBomberman.getSize().x, (float)window->getSize().y / textureBomberman.getSize().y);
		backgroundBomberman.setScale(resizeVal, resizeVal);
		backgroundSquares.setTextureRect({ window->getPosition().x, window->getPosition().y, (int)window->getSize().x, (int)window->getSize().y });

		mainMenuReprocessDisplay = true;
		playingReprocessDisplay = true;
		optionsMenuReprocessDisplay = true;
		pauseMenuReprocessDisplay = true;
		gameOverReprocessDisplay=true;
		multiplayerReprocessDisplay = true;
		storyReprocessDisplay = true;
		scoreReprocessDisplay = true;
		victoryReprocessDisplay = true;
		endBattleReprocessDisplay = true;
		gameInterfaceReprocessDisplay = true;
		colorPickerReprocessDisplay = true;
		tutorialReprocessDisplay = true;
	}

	void updateCamera() {
		window->setView(camera); 
	}

	/**
	 * Core sfml event handler. It can be call with a callback if more events are need to be watch by different
	 * interfaces
	 * @param gameDisplay
	 * @param menuButtonActions
	 */
    void manageGameInterface(GameDisplayController &gameDisplay, std::function<void(sf::Event&)> menuButtonActions = nullptr) {
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
				if (gameState == GameDisplayController::GameState::PLAYING) {
					GameTime::stopGameTime();
					gameDisplay.setGameState(GameDisplayController::GameState::PAUSE_MENU);
				}
				break;
			case sf::Event::GainedFocus:
				// Resume
				/*if (gameState == GameDisplayController::GameState::PAUSE_MENU) {
					GameTime::resumeGameTime();
					gameDisplay.setGameState(GameDisplayController::GameState::PLAYING);
				}*/
				break;
			case sf::Event::Resized: {
				// update the view to the new size of the window
				
				window->setSize(sf::Vector2u(event.size.width, event.size.height));
				gameDisplay.windowWidth = event.size.width;
				gameDisplay.windowHeight = event.size.height;
				gameDisplay.saveProperties();
				notifyChangeDisplay();
				
				camera.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
				camera.setViewport(sf::FloatRect(0.f, 0.1f, 1.f, 1.f));
				playingGuiView = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y / 5));
				playingGuiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.2f));

				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				menuView = sf::View(visibleArea);
				// sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				// window->setView(sf::View(visibleArea));
				window->clear();
				break;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//									   IF MENU BUTTON PRESSED OR OTHE CUSTOM
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
				if (menuButtonActions) {
					menuButtonActions(event);
				}
				break;
			}
		}
    }

	int getIntProperty(string propName) {
		
		if (gameProperties.iskey(propName)) {
			string propValString = gameProperties.at(propName);
			return stoi(propValString);
		}
		else {
			cerr << "Value in property file does not exists. Creating a new one." << endl;
			createNewPropertiesFile();
			return 0;
		}
	}

	void createNewPropertiesFile() {
		fstream propertiesFile;
		propertiesFile.open(PROPERTIES_FILENAME, std::fstream::in | std::fstream::out | fstream::trunc);
		propertiesFile << "volume.master = 50\n";
		propertiesFile << "volume.music = 50\n";
		propertiesFile << "volume.sound = 50\n\n";

		propertiesFile << "resolution.x = 1024\n";
		propertiesFile << "resolution.y = 768\n";
		propertiesFile << "fullscreen = 0\n";
		propertiesFile << "fps = 60\n";

		propertiesFile << "player1.up = " + to_string((int)sf::Keyboard::W) << endl;
		propertiesFile << "player1.down = " + to_string((int)sf::Keyboard::S) << endl;
		propertiesFile << "player1.right = " + to_string((int)sf::Keyboard::D) << endl;
		propertiesFile << "player1.left = " + to_string((int)sf::Keyboard::A) << endl;
		propertiesFile << "player1.bomb = " + to_string((int)sf::Keyboard::Space) << endl;
		propertiesFile << "player1.action = " + to_string((int)sf::Keyboard::E) << endl;

		propertiesFile << "player2.up = " + to_string((int)sf::Keyboard::Up) << endl;
		propertiesFile << "player2.down = " + to_string((int)sf::Keyboard::Down) << endl;
		propertiesFile << "player2.right = " + to_string((int)sf::Keyboard::Right) << endl;
		propertiesFile << "player2.left = " + to_string((int)sf::Keyboard::Left) << endl;
		propertiesFile << "player2.bomb = " + to_string((int)sf::Keyboard::RControl) << endl;
		propertiesFile << "player2.action = " + to_string((int)sf::Keyboard::RShift) << endl;

		propertiesFile << "tutorial.intro = 1"<< endl;
		propertiesFile << "tutorial.ability = 1 " << endl;

		propertiesFile.close();
	}

    /**
     * Read (or create if does not exits) the game properties file, and get last game properties set by user
     */
	UserKeyPress *getProperties() {
		fstream propertiesFile;

		propertiesFile.open(PROPERTIES_FILENAME, std::fstream::in);

		// If file does not exist, Create new file
		if (!propertiesFile.is_open()) {
			propertiesFile.close();
			createNewPropertiesFile();
		}
		else {
			propertiesFile.close();
		}

		propertiesFile.open(PROPERTIES_FILENAME, std::fstream::in | std::fstream::out);

		propertiesFile >> gameProperties;
		PropReader::data::const_iterator iter;

		// TODO: Remove this
		for (iter = gameProperties.begin(); iter != gameProperties.end(); iter++) {
			cout << iter->first << " = " << iter->second << endl;
		}

		//cout << gameProperties.at("volume.master") << endl;
		GameMusic::setMasterVolume(getIntProperty("volume.master"));
		GameMusic::setVolume(getIntProperty("volume.music"));
		GameSounds::setVolume(getIntProperty("volume.sound"));

		FPSs = getIntProperty("fps");
		fullScreen = (bool)getIntProperty("fullscreen");
		windowWidth = getIntProperty("resolution.x");
		windowHeight = getIntProperty("resolution.y");

		PlayerEntity::PlayerControls player1;
		player1.goUp = (sf::Keyboard::Key)getIntProperty("player1.up");
		player1.goDown = (sf::Keyboard::Key)getIntProperty("player1.down");
		player1.goRight = (sf::Keyboard::Key)getIntProperty("player1.right");
		player1.goLeft = (sf::Keyboard::Key)getIntProperty("player1.left");
		player1.UseBomb = (sf::Keyboard::Key)getIntProperty("player1.bomb");
		player1.MakeAction = (sf::Keyboard::Key)getIntProperty("player1.action");

		PlayerEntity::PlayerControls player2;
		player2.goUp = (sf::Keyboard::Key)getIntProperty("player2.up");
		player2.goDown = (sf::Keyboard::Key)getIntProperty("player2.down");
		player2.goLeft = (sf::Keyboard::Key)getIntProperty("player2.left");
		player2.goRight = (sf::Keyboard::Key)getIntProperty("player2.right");
		player2.UseBomb = (sf::Keyboard::Key)getIntProperty("player2.bomb");
		player2.MakeAction = (sf::Keyboard::Key)getIntProperty("player2.action");

		tutorialAbility = (bool)getIntProperty("tutorial.ability");
		tutorialIntro = (bool)getIntProperty("tutorial.intro");

		UserKeyPress * userKeyPressManager = new UserKeyPress(player1, player2);

		propertiesFile.close();

		return userKeyPressManager;
	}

	/**
	 * Save user changed properties in the properties file
	 */
	void saveProperties() {
		gameProperties.at("volume.master") = to_string(GameMusic::getMasterVolume());
		gameProperties.at("volume.music") = to_string(GameMusic::getVolume()); 
		gameProperties.at("volume.sound") = to_string(GameSounds::getVolume());

		gameProperties.at("fps") = to_string(FPSs);
		gameProperties.at("fullscreen") = to_string((int)fullScreen);
		gameProperties.at("resolution.x") = to_string(windowWidth);
		gameProperties.at("resolution.y") = to_string(windowHeight);

		gameProperties.at("player1.up") = to_string(userKeyPress->player1.goUp);
		gameProperties.at("player1.down") = to_string(userKeyPress->player1.goDown);
		gameProperties.at("player1.right") = to_string(userKeyPress->player1.goRight);
		gameProperties.at("player1.left") = to_string(userKeyPress->player1.goLeft);
		gameProperties.at("player1.bomb") = to_string(userKeyPress->player1.UseBomb);
		gameProperties.at("player1.action") = to_string(userKeyPress->player1.MakeAction);

		gameProperties.at("player2.up") = to_string(userKeyPress->player2.goUp);
		gameProperties.at("player2.down") = to_string(userKeyPress->player2.goDown);
		gameProperties.at("player2.right") = to_string(userKeyPress->player2.goRight);
		gameProperties.at("player2.left") = to_string(userKeyPress->player2.goLeft);
		gameProperties.at("player2.bomb") = to_string(userKeyPress->player2.UseBomb);
		gameProperties.at("player2.action") = to_string(userKeyPress->player2.MakeAction);

		gameProperties.at("tutorial.ability") = to_string((int)tutorialAbility);
		gameProperties.at("tutorial.intro") = to_string((int)tutorialIntro);

		fstream file;
		file.open(PROPERTIES_FILENAME, std::fstream::in | std::fstream::out | fstream::trunc);
		file << gameProperties;
	}
};