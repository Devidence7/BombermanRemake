#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class PauseMenu {
	GameGUI::Menu* menu;

	bool EsqPressed = false;

	enum ButtonActions {
		RESUME,
		SAVE,
		OPTIONS,
		GO_MAIN_MENU,
		QUIT
	};

	sf::Texture texture;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Slider* masterVolumenSlider;
	GameGUI::Slider* musicSlider;

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
	PauseMenu(sf::RenderWindow& window) {
		menu = new GameGUI::Menu(window);

		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ window.getPosition().x, window.getPosition().y, (int)window.getSize().x, (int)window.getSize().y });

		menu->addButton("                Reanudar                ", ButtonActions::RESUME);
		menu->addButton("                 Guardar                 ", ButtonActions::SAVE);
		menu->addButton("                 Opciones                 ", ButtonActions::OPTIONS);
		menu->addButton("        Ir al menu principal       ", ButtonActions::GO_MAIN_MENU);
		menu->addButton("                    Salir                    ", ButtonActions::QUIT);

		createBackgroundMenu(window);
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::RESUME:
			gameDisplay.setGameState(GameDisplayController::GameState::PLAYING);
			GameTime::resumeGameTime();
			break;

		case ButtonActions::SAVE:
					
			break;
				
		case ButtonActions::OPTIONS:
			OptionsMenu::lastGameStateOptionsMenu = GameDisplayController::GameState::PAUSE_MENU;
			gameDisplay.setGameState(GameDisplayController::GameState::OPTIONS_MENU);
			break;
				
		case ButtonActions::GO_MAIN_MENU:
			game.deleteMap();
			gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
			break;
				
		case ButtonActions::QUIT:
			window->close();
			break;
		}
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
	void menuActions(GameDisplayController& gameDisplay, Game game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&PauseMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.pauseMenuReprocessDisplay) {
			gameDisplay.pauseMenuReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}

	void checkUserPauseActions(GameDisplayController& gameDisplay) {
		if (gameDisplay.getGameState() == GameDisplayController::GameState::PLAYING || gameDisplay.getGameState() == GameDisplayController::GameState::PAUSE_MENU) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				if (!EsqPressed) {
					EsqPressed = true;
					if (gameDisplay.getGameState() == GameDisplayController::GameState::PAUSE_MENU) {
						GameTime::resumeGameTime();
						gameDisplay.setGameState(GameDisplayController::GameState::PLAYING);
					}
					else {
						GameTime::stopGameTime();
						gameDisplay.setGameState(GameDisplayController::GameState::PAUSE_MENU);
					}
				}
			}
			else {
				EsqPressed = false;
			}
		}
	}
};

