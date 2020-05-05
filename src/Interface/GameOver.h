#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class GameOver {
	GameGUI::Menu* menu;
	GameDisplayController::GameState lastGameState;
	bool EsqPressed = false;
    sf::Font font;

	enum ButtonActions {
		RETRY,
		GO_MAIN_MENU,
		QUIT
	};

	sf::Texture texture;
    sf::Text game_over;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Slider* masterVolumenSlider;
	GameGUI::Slider* musicSlider;

	void createBackgroundMenu(sf::RenderWindow& window) {
		menu->setPosition(sf::Vector2f((int)window.getSize().x / 2 - (int)menu->getSize().x / 2, (int)window.getSize().y / 2 - (int)menu->getSize().y / 2));
        if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }
        
        
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
	GameOver(sf::RenderWindow& window) {
		menu = new GameGUI::Menu(window);
    

		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });

       
		menu->addButton("                Reintentar               ", ButtonActions::RETRY);
		menu->addButton("        Ir al menu principal       ", ButtonActions::GO_MAIN_MENU);
		menu->addButton("                    Salir                    ", ButtonActions::QUIT);

		createBackgroundMenu(window);

         game_over.setFont(font);
        game_over.setString("       GAME OVER");
        game_over.setPosition(sf::Vector2f((int)window.getSize().x / 2 - (int)menu->getSize().x / 2, ((int)window.getSize().y / 2 - (int)menu->getSize().y / 2)-150));
             game_over.setFillColor(sf::Color::White);
       // game_over.setColor(sf::Color::Black);
        game_over.setScale(2,2);
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::RETRY:
			gameDisplay.setGameState(GameDisplayController::GameState::RESTART);
			break;
				
		case ButtonActions::GO_MAIN_MENU:
			gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
			break;
				
		case ButtonActions::QUIT:
			window->close();
			break;
		}
	}

	void draw(sf::RenderWindow& window) {
		window.draw(background);
        window.draw(game_over);
		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow1);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);

		window.draw(*menu);
	}

public:
	void menuActions(GameDisplayController& gameDisplay, Game game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&GameOver::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.gameOverReprocessDisplay) {
			gameDisplay.gameOverReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}

	void checkUserGameOverActions(GameDisplayController& gameDisplay) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			if (!EsqPressed) {
				EsqPressed = true;
				if (gameDisplay.getGameState() == GameDisplayController::GameState::GAME_OVER) {
					gameDisplay.setGameState(lastGameState);
				}
				else {
					lastGameState = gameDisplay.getGameState();
					gameDisplay.setGameState(GameDisplayController::GameState::GAME_OVER);
				}
			}
		}
		else {
			EsqPressed = false;
		}
	}

};

