#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class AllVsAllMenu {
	GameGUI::Menu* menu;

	bool EsqPressed = false;

	enum ButtonActions {
        //Dos jugadores
        PLVSIA,
        PLVSPL,
        //Tres jugadores
		PL1VS2IA,
        PL2VS1IA,
        //Cuatro jugadores
		PL1VS3IA,
        PL2VS2IA,
        //
        BACK,
		QUIT

	};

	sf::Texture texture;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;

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
	AllVsAllMenu(sf::RenderWindow& window) {
		menu = new GameGUI::Menu(window);

		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });


		
	}

	void actualizeNumPlayers(sf::RenderWindow& window,int numPlayers){
		cout<<"NumPLayers: "<<numPlayers<<endl;
			 switch ((numPlayers))
        {
        case 2:
        menu->addButton("               Jugador vs IA               ", ButtonActions:: PLVSIA);
		menu->addButton("                  Jugador vs Jugador                ", ButtonActions::PLVSPL);
		menu->addButton("          Atras       ", ButtonActions::BACK);
		menu->addButton("                    Salir                    ", ButtonActions::QUIT);
            break;

        case 3:
        menu->addButton("               1  Jugador , 2 IA                ", ButtonActions::PL1VS2IA);
		menu->addButton("               2  Jugadores , 1 IA                 ", ButtonActions::PL2VS1IA);
		menu->addButton("         Atras        ", ButtonActions::BACK);
		menu->addButton("                    Salir                    ", ButtonActions::QUIT);
            break;

        case 4:
            menu->addButton("            1 Jugador , 3 IA                ", ButtonActions::PL1VS3IA);
		menu->addButton("                2 Jugadores , 2 IA                 ", ButtonActions::PL2VS2IA);
		menu->addButton("          Atrás        ", ButtonActions::BACK);
		menu->addButton("                    Salir                    ", ButtonActions::QUIT);
            break;
        
        default:
            break;
        }
		createBackgroundMenu(window);
	}
	void menuActions(GameDisplayController& gameDisplay, Game game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&AllVsAllMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.pauseMenuReprocessDisplay) {
			gameDisplay.pauseMenuReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::PLVSIA:
        
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

		case ButtonActions::PLVSPL:
        	gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
					
			break;
				
		case ButtonActions::PL1VS2IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PL2VS1IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PL1VS3IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;
        case ButtonActions::PL2VS2IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

       	case ButtonActions::BACK:
			//window->close();
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

	

	
};

