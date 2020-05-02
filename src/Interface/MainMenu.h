#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>
#include "GUI/GameGUI.hpp"
#include "GUI/Theme.hpp"
#include "../Music/GameMusic.h"

#include "InterfacePaths.h"
#include "GameInterface.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"

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

	void createMainMenu(sf::RenderWindow& window) {
		texture.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
		background.setTexture(texture);

		background.setScale((float)window.getSize().x / texture.getSize().x, (float)window.getSize().x / texture.getSize().x);

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

public:
	MainMenu(sf::RenderWindow& window) {
		GameGUI::Theme::loadFont("../textures/mainMenu/BOMBERMAN.ttf");
		GameGUI::Theme::loadTexture("../textures/interface/bomberman.png");
		GameGUI::Theme::textCharacterSize = 26;
		GameGUI::Theme::click.textColor = sf::Color::Black;
		GameGUI::Theme::click.textColorHover = sf::Color::Red;
		GameGUI::Theme::click.textColorFocus = sf::Color::Red;
		GameGUI::Theme::PADDING = 10.f;

		createMainMenu(window);
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
				// catch the resize events
			case sf::Event::Resized: {
					// update the view to the new size of the window
					sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
					window.setView(sf::View(visibleArea));
					createMainMenu(window);
					break;
				}
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
					OptionsMenu::lastGameStateOptionsMenu = GameInterface::GameState::MAIN_MENU;
					gameState = GameInterface::GameState::OPTIONS_MENU;
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
		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);
		window.draw(*menu);
	}

	void menuActions(sf::RenderWindow& window, GameInterface::GameState& gameState, Game game) {
		userActions(window, gameState, game);
		draw(window);
	}
};
