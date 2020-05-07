#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class MultiplayerMenu {
	GameGUI::Menu* menu;

	bool EsqPressed = false;

	enum ButtonActions {
		//NUmJUGADORES
		TWOPLAYERS,
		THREEPLAYERS,
		FOURPLAYERS,
    
        //TIPO DE JUEGO
        ALLVSALL,
        TEAM,

	//VERSUS
	//Dos jugadores
        //Dos jugadores
        PLVSIA,
        PLVSPL,
        //Tres jugadores
		PL12IA,
        PL21IA,
        //Cuatro jugadores
		PL13IA,
        PL22IA,

	//TEAM
        
        //Tres jugadores
		PL1VS2IA,
        PLVSPLIA,
        PL2VS1IA,
        //Cuatro jugadores
		PL1VS3IA,
        PLVSPL2IA,
        PL2VS2IA,
        PLIAVSPLIA,
        //
        BACK,
		QUIT

	};

	enum MenuState{
		MAIN=1,
		NUM_PLAYERS_SEL=2,
		TYPE_OF_GAME=3,
	};

	sf::Texture texture;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;
	MultiplayerMenu::MenuState previousMenu;
	static GameDisplayController::GameState lastGameStateOptionsMenu;

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
	MultiplayerMenu(sf::RenderWindow& window,GameDisplayController& gameDisplay) {
		previousMenu=MenuState::MAIN;
		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });
		createNumPlaySelection(window,gameDisplay);
		
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::TWOPLAYERS:
        	game.gameOptions.numPlayers=2;
			previousMenu=MenuState::NUM_PLAYERS_SEL;
			createAllVSAllMenu(*window,gameDisplay,game.gameOptions.numPlayers);
			break;

		case ButtonActions::THREEPLAYERS:
			
        	game.gameOptions.numPlayers=3;
			previousMenu=MenuState::NUM_PLAYERS_SEL;
        	createGameTypeMenu(*window,gameDisplay);
					
			break;
				
		case ButtonActions::FOURPLAYERS:
			
        	game.gameOptions.numPlayers=4;
			previousMenu=MenuState::NUM_PLAYERS_SEL;
			//OptionsMenu::lastGameStateOptionsMenu = GameDisplayController::GameState::MULTIPLAYER_MENU;
			createGameTypeMenu(*window,gameDisplay);
			break;

		case ButtonActions::ALLVSALL:
			previousMenu=MenuState::TYPE_OF_GAME;
			createAllVSAllMenu(*window,gameDisplay,game.gameOptions.numPlayers);
			break;

		case ButtonActions::TEAM:
			previousMenu=MenuState::TYPE_OF_GAME;
        	createTeamVSTeamMenu(*window,gameDisplay,game.gameOptions.numPlayers);		
			break;

		case ButtonActions::PLVSIA:
        
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

		case ButtonActions::PLVSPL:
        	gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
					
			break;
				
		case ButtonActions::PL12IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PL21IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PL13IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;
        case ButtonActions::PL22IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

		
		case ButtonActions::PL1VS2IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PLVSPLIA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PL2VS1IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        //4 players

        case ButtonActions::PL1VS3IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;
        case ButtonActions::PLVSPL2IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

        case ButtonActions::PL2VS2IA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;
        case ButtonActions::PLIAVSPLIA:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

				
		case ButtonActions::BACK:
			switch (previousMenu)
			{
			case MAIN:
				gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
				break;

			case NUM_PLAYERS_SEL:
				previousMenu=MenuState::MAIN;
				createNumPlaySelection(*window,gameDisplay);
				break;
			
			case TYPE_OF_GAME:
				previousMenu=MenuState::NUM_PLAYERS_SEL;
				createGameTypeMenu(*window,gameDisplay);
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

	void createNumPlaySelection(sf::RenderWindow& window, GameDisplayController &gameDisplay){
		menu = new GameGUI::Menu(window);

		menu->addButton("                2 jugadores               ", ButtonActions::TWOPLAYERS);
		menu->addButton("                3 jugadores                 ", ButtonActions::THREEPLAYERS);
		menu->addButton("                4 jugadores                 ", ButtonActions::FOURPLAYERS);
		menu->addButton("                   Atras       ", ButtonActions::BACK);

		createBackgroundMenu(window);
	}

	void createGameTypeMenu(sf::RenderWindow& window, GameDisplayController &gameDisplay){

			menu = new GameGUI::Menu(window);

		menu->addButton("                Todos contra todos              ", ButtonActions::ALLVSALL);
		menu->addButton("                Batalla por equipos                 ", ButtonActions::TEAM);
		menu->addButton("            		    Atras      ", ButtonActions::BACK);

		createBackgroundMenu(window);

	}


	void createAllVSAllMenu(sf::RenderWindow& window, GameDisplayController &gameDisplay,int numPlayers){
			menu = new GameGUI::Menu(window);
			 switch ((numPlayers))
        {
        case 2:
        	menu->addButton("                   Jugador vs IA               ", ButtonActions:: PLVSIA);
			menu->addButton("                  Jugador vs Jugador                ", ButtonActions::PLVSPL);
			menu->addButton("                        Atras       ", ButtonActions::BACK);
            break;

        case 3:
       		menu->addButton("               1  Jugador , 2 IA                ", ButtonActions::PL12IA);
			menu->addButton("               2  Jugadores , 1 IA                 ", ButtonActions::PL21IA);
			menu->addButton("                      Atras        ", ButtonActions::BACK);
            break;

        case 4:
        	menu->addButton("                 1 Jugador , 3 IA                ", ButtonActions::PL13IA);
			menu->addButton("                2 Jugadores , 2 IA                 ", ButtonActions::PL22IA);
			menu->addButton("                        Atras        ", ButtonActions::BACK);
            break;
        
        default:
            break;
        }
		createBackgroundMenu(window);
	}



	void createTeamVSTeamMenu(sf::RenderWindow& window, GameDisplayController &gameDisplay,int numPlayers){
		menu = new GameGUI::Menu(window);
		
			  switch ((numPlayers))
        {
        
        case 3:
        menu->addButton("                 1 Jugador Vs 2 IA                ", ButtonActions::PL1VS2IA);
		menu->addButton("                 2 Jugadores Vs 1 IA                 ", ButtonActions::PL2VS1IA);
        menu->addButton("                1 Jugador vs Jugador IA                ", ButtonActions::PLVSPLIA);
		menu->addButton("                          Atras        ", ButtonActions::BACK);
	
            break;

        case 4:
        menu->addButton("           	1 Jugador Vs  3 IA                ", ButtonActions::PL1VS3IA);
		menu->addButton("          	 1 Jugadores Vs 2 Jugadores 1 IA                 ", ButtonActions::PLVSPL2IA);
        menu->addButton("          	    2 Jugadores Vs 2 IA                ", ButtonActions::PL2VS2IA);
		menu->addButton("             Jugador IA  Vs Jugador IA                 ", ButtonActions::PLIAVSPLIA);
		menu->addButton("                        Atras        ", ButtonActions::BACK);
            break;
        
        default:
            break;
        }
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
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&MultiplayerMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.pauseMenuReprocessDisplay) {
			gameDisplay.pauseMenuReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}

	
};

