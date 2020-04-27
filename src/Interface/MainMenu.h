#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>
#include "GUI/GameGUI.hpp"
#include "GUI/Theme.hpp"
#include "../Music/GameMusic.h"

#include "InterfacePaths.h"
#include "GameInterface.h"
#include "../GameEngine.hpp"
#include "PauseMenu.h"


using namespace GameGUI;

class MainMenu {
	sf::Sprite background;
	sf::Texture texture;

	GameGUI::Menu* menu;

	enum ButtonActions {
		SINGLEPLAYER,
		MULTIPLAYER,
		OPCIONS,
		QUIT
	};

public:
	MainMenu(sf::RenderWindow& window) {
		texture.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
		background.setTexture(texture);

		background.setScale((float)window.getSize().x / texture.getSize().x, (float)window.getSize().x / texture.getSize().x);

		menu = new GameGUI::Menu(window);
		menu->setPosition(sf::Vector2f(100, 500));

		GameGUI::Theme::loadFont("../textures/mainMenu/BOMBERMAN.ttf");
		GameGUI::Theme::loadTexture("../textures/interface/bomberman.png");
		GameGUI::Theme::textCharacterSize = 26;
		GameGUI::Theme::click.textColor = sf::Color::Black;
		GameGUI::Theme::click.textColorHover = sf::Color::Red;
		GameGUI::Theme::click.textColorFocus = sf::Color::Red;
		GameGUI::Theme::PADDING = 10.f;

		menu->addButton("Un Jugador", ButtonActions::SINGLEPLAYER);
		menu->addButton("Multijugador", ButtonActions::MULTIPLAYER);
		menu->addButton("Opciones", ButtonActions::OPCIONS);
		menu->addButton("Salir", ButtonActions::QUIT);
	}


private:
	void userActions(sf::RenderWindow& window, GameInterface::GameState& gameState, Game game) {
		sf::Event event;
		while (window.pollEvent(event)) {
			// Process events
			switch (event.type) {
				// window closed
			case sf::Event::Closed:
				// Close window -> exit
				window.close();
				break;
			case sf::Event::LostFocus:
				// Pause
				break;
			case sf::Event::GainedFocus:
				// Resume
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//													BUTTON PRESSED
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
				int id = menu->onEvent(event);
				switch (id) {
				case ButtonActions::SINGLEPLAYER:
					gameState = GameInterface::GameState::PLAYING;
					game.gameOptions.multiplayerGame = false;
					game.startNewGame(window);
					break;
				case ButtonActions::MULTIPLAYER:
					gameState = GameInterface::GameState::PLAYING;
					game.gameOptions.multiplayerGame = true;
					game.startNewGame(window);
					break;
				case ButtonActions::OPCIONS:
					gameState = GameInterface::GameState::LITTLE_MENU;
					
					break;
				case ButtonActions::QUIT:
					window.close();
					break;
				}
			}
		}
	}


public:
	void draw(sf::RenderWindow& window) {
		window.draw(background);
		window.draw(*menu);
	}

	void menuActions(sf::RenderWindow& window, GameInterface::GameState& gameState, Game game) {
		userActions(window, gameState, game);
		draw(window);
	}
};
