#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class VictoryScreen {
	GameGUI::Menu* menu;
	GameDisplayController::GameState lastGameState;
	bool EsqPressed = false;
    sf::Font font;

	enum ButtonActions {
		CONTINUE,
		GO_MAIN_MENU,
		QUIT
	};

	sf::Texture texture;
    sf::Text level;
   sf::Text victory;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Slider* masterVolumenSlider;
	GameGUI::Slider* musicSlider;

	void createBackgroundMenu(sf::RenderWindow& window) {
	//	menu->setPosition(sf::Vector2f((int)window.getSize().x / 2 - (int)menu->getSize().x / 2, (int)window.getSize().y / 2 - (int)menu->getSize().y / 2));
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
	VictoryScreen(sf::RenderWindow& window,Game &game) {
		menu = new GameGUI::Menu(window);

        victory.setFont(font);
        victory.setString("    VICTORIA    ");
        victory.setPosition(sf::Vector2f(window.getSize().x / 2-(window.getSize().x/9),window.getSize().y / 2-(window.getSize().y/9)-150));
        victory.setColor(sf::Color::White);
        victory.setScale(2,2);

        level.setFont(font);
        level.setColor(sf::Color::White);
        level.setString("NIVEL " + to_string(game.stage)+": Completado");
        level.setPosition(sf::Vector2f(window.getSize().x / 2-(window.getSize().x/9) ,victory.getPosition().y+150));
        level.setScale(1.5,1.5);
    

		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
        background.setColor(sf::Color(0, 0, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });

        menu->setPosition(sf::Vector2f(level.getPosition().x-50,level.getPosition().y+150));

       
		menu->addButton("                Continuar               ", ButtonActions::CONTINUE);
		menu->addButton("           Ir al menu principal       ", ButtonActions::GO_MAIN_MENU);
		menu->addButton("                   Salir                    ", ButtonActions::QUIT);

		createBackgroundMenu(window);

        
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::CONTINUE:
            game.passLevel();
            
		    game.deleteMap();
            updateLevel(game);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);		
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
        window.draw(victory);
        window.draw(level);
		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow1);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);

		window.draw(*menu);
	}

public:
	void menuActions(GameDisplayController& gameDisplay, Game &game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&VictoryScreen::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.gameOverReprocessDisplay) {
			gameDisplay.gameOverReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}

    void updateLevel(Game &game){
        level.setString("NIVEL " + to_string(game.stage)+": Completado");
    }
};

