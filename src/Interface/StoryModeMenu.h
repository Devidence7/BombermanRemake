#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class StoryModeMenu {
	GameGUI::Menu* menu;

	bool EsqPressed = false;

	enum ButtonActions {
		SINGLEPLAYER,
		COOP,
		EASYLEVEL,
		NORMALLEVEL,
		HARDLEVEL,
        BACK,
		QUIT
	};

	enum MenuState{
		MAIN,
		NUMPLAYERS
	};

	sf::Texture texture;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;
	StoryModeMenu::MenuState previousMenu;

	void createBackgroundMenu(sf::RenderWindow& window) {
		menu->setPosition(sf::Vector2f((int)window.getSize().x / 2 - (int)menu->getSize().x / 2, (int)window.getSize().y / 2 - (int)menu->getSize().y / 2));

		float menuBackgroundPadding = 50;
		menuBackground.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding, menu->getSize().y + 2 * menuBackgroundPadding));
		menuBackground.setPosition(menu->getPosition().x - menuBackgroundPadding, menu->getPosition().y - menuBackgroundPadding);
		menuBackground.setFillColor(sf::Color(255, 255, 153, 200));

		menuBackgroundShadow.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding - 4, menu->getSize().y + 2 * menuBackgroundPadding - 4));
		menuBackgroundShadow.setPosition(menu->getPosition().x - menuBackgroundPadding + 12, menu->getPosition().y - menuBackgroundPadding + 12);
		menuBackgroundShadow.setFillColor(sf::Color(15, 35, 35, 60));

		menuBackgroundShadow1.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding - 2, menu->getSize().y + 2 * menuBackgroundPadding - 2));
		menuBackgroundShadow1.setPosition(menu->getPosition().x - menuBackgroundPadding + 11, menu->getPosition().y - menuBackgroundPadding + 11);
		menuBackgroundShadow1.setFillColor(sf::Color(15, 35, 35, 20));

		menuBackgroundShadow2.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding, menu->getSize().y + 2 * menuBackgroundPadding));
		menuBackgroundShadow2.setPosition(menu->getPosition().x - menuBackgroundPadding + 10, menu->getPosition().y - menuBackgroundPadding + 10);
		menuBackgroundShadow2.setFillColor(sf::Color(15, 35, 35, 20));
	}

public:
	StoryModeMenu(sf::RenderWindow& window) {
		previousMenu=MenuState::MAIN;

		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });
		createNumPlayersMenu(window);

	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::SINGLEPLAYER:
			previousMenu=MenuState::NUMPLAYERS;
        	game.gameOptions.numPlayers=1;
			createDifficultyMenu(*window);

			
			break;

		case ButtonActions::COOP:
			previousMenu=MenuState::NUMPLAYERS;
        	game.gameOptions.numPlayers=2;
			createDifficultyMenu(*window);
        	
					
			break;

		case ButtonActions::EASYLEVEL:

            game.gameOptions.difLevel=1.25;
			previousMenu=MenuState::MAIN;
				createNumPlayersMenu(*window);
			gameDisplay.setGameState(GameDisplayController::GameState::PICK_COLOR);
			gameDisplay.notifyChangeDisplay();
			break;

		case ButtonActions::NORMALLEVEL:
            game.gameOptions.difLevel=1.5;
			previousMenu=MenuState::MAIN;
				createNumPlayersMenu(*window);
			createNumPlayersMenu(*window);
        	gameDisplay.setGameState(GameDisplayController::GameState::PICK_COLOR);
			gameDisplay.notifyChangeDisplay();
					
			break;
				
		case ButtonActions::HARDLEVEL:
            game.gameOptions.difLevel=1.75;
			previousMenu=MenuState::MAIN;
				createNumPlayersMenu(*window);
			//OptionsMenu::lastGameStateOptionsMenu = GameDisplayController::GameState::MULTIPLAYER_MENU;
			gameDisplay.setGameState(GameDisplayController::GameState::PICK_COLOR);
			gameDisplay.notifyChangeDisplay();
			break;
		

			
		case ButtonActions::BACK:
		switch(previousMenu){
			case MAIN:
				gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
				break;
			case NUMPLAYERS:
				previousMenu=MenuState::MAIN;
				createNumPlayersMenu(*window);
				break;
			default:
				break;		
		}
		break;

			case ButtonActions::QUIT:
			window->close();
			break;
		}
	}

	void createDifficultyMenu(sf::RenderWindow& window){
		menu = new GameGUI::Menu(window);
		GameGUI::HorizontalBoxLayout* hboxQuit = menu->addHorizontalBoxLayout();
		menu->addButton("                  Facil                   ", ButtonActions::EASYLEVEL);
		menu->addButton("                Normal                 ", ButtonActions::NORMALLEVEL);
		menu->addButton("                 Dificil                   ", ButtonActions::HARDLEVEL);
		hboxQuit->addButton("Atras", ButtonActions::BACK);

		createBackgroundMenu(window);
	}


	void createNumPlayersMenu(sf::RenderWindow& window){
		menu = new GameGUI::Menu(window);
		GameGUI::HorizontalBoxLayout* hboxQuit = menu->addHorizontalBoxLayout();
		menu->addButton("                  1 jugador                   ", ButtonActions::SINGLEPLAYER);
		menu->addButton("                2 jugadores                 ", ButtonActions::COOP);
		hboxQuit->addButton("Atras", ButtonActions::BACK);
		createBackgroundMenu(window);
	}

	void draw(sf::RenderWindow& window) {
		

		window.draw(background);

		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow1);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);

		window.draw(*menu);
	}

public:
	void menuActions(GameDisplayController& gameDisplay, Game& game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&StoryModeMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.storyReprocessDisplay) {
			gameDisplay.storyReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}
};

