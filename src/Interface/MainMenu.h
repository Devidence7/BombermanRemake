#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>

#include "GUI/GameGUI.hpp"

#include "InterfacePaths.h"
#include "GameInterface.h"
#include "OptionsMenu.h"
#include "GameDisplayController.h"
#include "../GameEngine.hpp"
#include "../Music/GameMusic.h"


using namespace GameGUI;

class MainMenu {
	sf::Sprite background;
	sf::Texture texture;

	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Menu* menu;

	enum ButtonActions {
		SINGLEPLAYER,
		MULTIPLAYER,
		OPCIONS,
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

		menu->addButton("           Modo historia            ", ButtonActions::SINGLEPLAYER);
		menu->addButton("           Modo batalla            ", ButtonActions::MULTIPLAYER);
		menu->addButton("               Opciones                 ", ButtonActions::OPCIONS);
		menu->addButton("                  Salir                    ", ButtonActions::QUIT);

		menu->setPosition(sf::Vector2f(window.getSize().x / 8 * 1.2, window.getSize().y - menu->getSize().y - window.getSize().x / 8));

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
	void userActions(sf::Event& event, sf::RenderWindow* &window, GameDisplayController& gameDisplay, Game& game) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//													BUTTON PRESSED
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::SINGLEPLAYER:
			gameDisplay.setGameState(GameDisplayController::GameState::PLAYING);
			game.gameOptions.numPlayers = 1;
			game.startNewGame(*window, gameDisplay);
			break;
		case ButtonActions::MULTIPLAYER:
			gameDisplay.setGameState(GameDisplayController::GameState::PLAYING);
			game.gameOptions.numPlayers = 2;
			game.startNewGame(*window, gameDisplay);
			break;
		case ButtonActions::OPCIONS:
			OptionsMenu::lastGameStateOptionsMenu = GameDisplayController::GameState::MAIN_MENU;
			gameDisplay.setGameState(GameDisplayController::GameState::OPTIONS_MENU);
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
	void menuActions(GameDisplayController &gameDisplay, Game& game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&MainMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.mainMenuReprocessDisplay) {
			gameDisplay.mainMenuReprocessDisplay = false;
			createMainMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}
};
