#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"
#include "InterfacePaths.h"


using namespace GameGUI;

class MainMenu {
	sf::Sprite background;
	sf::Texture texture;

	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Menu* menu;

	enum ButtonActions {
		STORY_MODE,
		VERSUS_MODE,
		DEBUG,
		OPCIONS,
		HISTORY_SCORE,
		QUIT
	};

public:

	MainMenu(sf::RenderWindow& window) {
		createMainMenu(window);
	}

	/**
	 * Creates the main menu user interface
	 * @param window
	 */
	void createMainMenu(sf::RenderWindow& window) {
		texture.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
		background.setTexture(texture);

		float resizeVal = std::fmax((float)window.getSize().x / texture.getSize().x, (float)window.getSize().y / texture.getSize().y);
		background.setScale(resizeVal, resizeVal);

		menu = new GameGUI::Menu(window);

		menu->addButton("           Modo historia            ", ButtonActions::STORY_MODE);
		menu->addButton("           Modo batalla            ", ButtonActions::VERSUS_MODE);
		//menu->addButton("      Modo sin muros para Victor       ", ButtonActions::DEBUG);
		menu->addButton("               Opciones                 ", ButtonActions::OPCIONS);
		menu->addButton("           Puntuaciones             ", ButtonActions::HISTORY_SCORE);
		menu->addButton("                  Salir                    ", ButtonActions::QUIT);

		menu->setPosition(sf::Vector2f(window.getSize().x / 8 * 1.2, window.getSize().y / 2));

		float menuBackgroundPadding = window.getSize().x / 32;
		menuBackground.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding, menu->getSize().y + 2 * menuBackgroundPadding));
		menuBackground.setPosition(menu->getPosition().x - menuBackgroundPadding, menu->getPosition().y - menuBackgroundPadding);
		menuBackground.setFillColor(sf::Color(255, 255, 255, 40));

		menuBackgroundShadow.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding + 8, menu->getSize().y + 2 * menuBackgroundPadding + 8));
		menuBackgroundShadow.setPosition(menu->getPosition().x - menuBackgroundPadding - 4, menu->getPosition().y - menuBackgroundPadding - 4);
		menuBackgroundShadow.setFillColor(sf::Color(255, 255, 255, 20));

		menuBackgroundShadow2.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding + 16, menu->getSize().y + 2 * menuBackgroundPadding + 16));
		menuBackgroundShadow2.setPosition(menu->getPosition().x - menuBackgroundPadding - 8, menu->getPosition().y - menuBackgroundPadding - 8);
		menuBackgroundShadow2.setFillColor(sf::Color(255, 255, 255, 20));
	}

private:
	/**
	 * Method callback for more event managing
	 * @param event
	 * @param window
	 * @param gameDisplay
	 * @param game
	 */
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//													BUTTON PRESSED
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::STORY_MODE:
			game.samePlay = false;
			game.gameOptions.numIAPlayers = 0;
			game.gameOptions.historyMode = true;
			game.debug = false;
			game.scoreForTheMoment = 0;
			gameDisplay.camera = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
			gameDisplay.setGameState(GameDisplayController::GameState::STORY_MENU);
			//game.startNewGame(*window);
			break;
		case ButtonActions::VERSUS_MODE:
			//  game.gameOptions.numPlayers = 2;
			game.samePlay = false;
			game.gameOptions.numIAPlayers = 0;
			game.gameOptions.historyMode = false;
			game.debug = false;
			game.scoreForTheMoment = 0;
			gameDisplay.setGameState(GameDisplayController::GameState::VS_NUMPLAYERS_MENU);

			break;

		case ButtonActions::DEBUG:
			
			game.gameOptions.numPlayers = 1;
			game.gameOptions.numIAPlayers = 1;
			game.gameOptions.difLevel = 1;
			game.debug = true;
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			//game.startNewGame(*window);
			break;


		case ButtonActions::OPCIONS:
			OptionsMenu::lastGameStateOptionsMenu = GameDisplayController::GameState::MAIN_MENU;
			gameDisplay.setGameState(GameDisplayController::GameState::OPTIONS_MENU);
			break;

		case ButtonActions::HISTORY_SCORE:
			gameDisplay.setGameState(GameDisplayController::FINAL_SCORE);
			gameDisplay.notifyChangeDisplay();
			break;

		case ButtonActions::QUIT:
			window->close();
			break;
		}
	}

	/**
	 * Draw Main menu interface
	 * @param window
	 */
	void draw(sf::RenderWindow& window) {
		window.draw(background);
		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);
		window.draw(*menu);
	}

public:

	/**
	 * Main main menu manage method (call every iteration)
	 * @param gameDisplay
	 * @param game
	 */
	void menuActions(GameDisplayController& gameDisplay, Game& game) {
		if (gameDisplay.mainMenuReprocessDisplay) {
			gameDisplay.mainMenuReprocessDisplay = false;
			createMainMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());

		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&MainMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
	}
};
