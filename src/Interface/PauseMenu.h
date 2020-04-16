#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include "../Include/global.hpp"
#include <iostream>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#define MAX_NUMBER_OF_ITEMS 3
using namespace std;

class PauseMenu {	
	GameGUI::Menu* menu;

	enum ButtonActions {
		AUDIO,
		GRAPHICS,
		CONTROLS,
		QUIT
	};

public:
	PauseMenu(sf::RenderWindow& window){
		menu = new GameGUI::Menu(window);
		menu->setPosition(sf::Vector2f(250, 250));

		GameGUI::Theme::loadFont("../textures/mainMenu/BOMBERMAM.ttf");
		GameGUI::Theme::loadTexture("../textures/interface/round.png");
		GameGUI::Theme::textCharacterSize = 26;
		GameGUI::Theme::click.textColor = sf::Color::Black;
		GameGUI::Theme::click.textColorHover = sf::Color::Red;
		GameGUI::Theme::click.textColorFocus = sf::Color::Red;
		GameGUI::Theme::PADDING = 10.f;

		menu->addButton("Audio", ButtonActions::AUDIO);
		menu->addButton("Graficos", ButtonActions::GRAPHICS);
		menu->addButton("Controles", ButtonActions::CONTROLS);
		menu->addButton("Atras", ButtonActions::QUIT);
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
				case ButtonActions::AUDIO:
					break;
				case ButtonActions::GRAPHICS:
					break;
				case ButtonActions::CONTROLS:
					break;
				case ButtonActions::QUIT:
					gameState = GameInterface::GameState::MAIN_MENU;
					break;
				}
			}
		}
	}

	void draw(sf::RenderWindow& window) {
		window.clear(GameGUI::Theme::windowBgColor);  //window.draw(background);
		window.draw(*menu);
	}

public:
	void menuActions(sf::RenderWindow& window, GameInterface::GameState& gameState, Game game) {
		userActions(window, gameState, game);
		draw(window);
	}
};
