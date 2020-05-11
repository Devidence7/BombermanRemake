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

	enum MenuState {
		MAIN = 1,
		NUM_PLAYERS_SEL = 2,
		TYPE_OF_GAME = 3,
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
	MultiplayerMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		previousMenu = MenuState::MAIN;
		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });
		createNumPlaySelection(window, gameDisplay);

	}

private:
	void fillPlayersAndTeamsArray(Game& game, int player1, int player2, int player3, int player4, int numTeams) {
		game.gameOptions.playersAndTeams[0] = player1;
		game.gameOptions.playersAndTeams[1] = player2;
		game.gameOptions.playersAndTeams[2] = player3;
		game.gameOptions.playersAndTeams[3] = player4;
		game.gameOptions.numTeams = numTeams;
	}

	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::TWOPLAYERS:
			game.gameOptions.numPlayers = 2;
			previousMenu = MenuState::NUM_PLAYERS_SEL;
			createAllVSAllMenu(*window, gameDisplay, game.gameOptions.numPlayers);
			break;

		case ButtonActions::THREEPLAYERS:

			game.gameOptions.numPlayers = 3;
			previousMenu = MenuState::NUM_PLAYERS_SEL;
			createGameTypeMenu(*window, gameDisplay);

			break;

		case ButtonActions::FOURPLAYERS:

			game.gameOptions.numPlayers = 4;
			previousMenu = MenuState::NUM_PLAYERS_SEL;
			//OptionsMenu::lastGameStateOptionsMenu = GameDisplayController::GameState::MULTIPLAYER_MENU;
			createGameTypeMenu(*window, gameDisplay);
			break;

		case ButtonActions::ALLVSALL:
			previousMenu = MenuState::TYPE_OF_GAME;
			createAllVSAllMenu(*window, gameDisplay, game.gameOptions.numPlayers);
			fillPlayersAndTeamsArray(game, 0, 1, 2, 3, 0);
			break;

		case ButtonActions::TEAM:
			previousMenu = MenuState::TYPE_OF_GAME;
			createTeamVSTeamMenu(*window, gameDisplay, game.gameOptions.numPlayers);
			break;

		case ButtonActions::PLVSIA:
			game.gameOptions.numPlayers = 1;
			game.gameOptions.numIAPlayers = 1;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

		case ButtonActions::PLVSPL:
			game.gameOptions.numPlayers = 2;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);

			break;

		case ButtonActions::PL12IA:
			game.gameOptions.numPlayers = 1;
			game.gameOptions.numIAPlayers = 2;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

		case ButtonActions::PL21IA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 1;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;

		case ButtonActions::PL13IA:
			game.gameOptions.numPlayers = 1;
			game.gameOptions.numIAPlayers = 3;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;
		case ButtonActions::PL22IA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 2;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;


		case ButtonActions::PL1VS2IA:
			game.gameOptions.numPlayers = 1;
			game.gameOptions.numIAPlayers = 2;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 1, 1, 1, 2);
			break;

		case ButtonActions::PLVSPLIA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 1;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 1, 1, 1, 2);
			break;

		case ButtonActions::PL2VS1IA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 1;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 0, 1, 1, 2);
			break;

			//4 players

		case ButtonActions::PL1VS3IA:
			game.gameOptions.numPlayers = 1;
			game.gameOptions.numIAPlayers = 3;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 1, 1, 1, 2);
			break;
		case ButtonActions::PLVSPL2IA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 1;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 1, 1, 1, 2);
			break;

		case ButtonActions::PL2VS2IA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 2;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 0, 1, 1, 2);
			break;
		case ButtonActions::PLIAVSPLIA:
			game.gameOptions.numPlayers = 2;
			game.gameOptions.numIAPlayers = 2;
			previousMenu = MenuState::MAIN;
			createNumPlaySelection(*window, gameDisplay);
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			fillPlayersAndTeamsArray(game, 0, 1, 0, 1, 2);
			break;


		case ButtonActions::BACK:
			switch (previousMenu) {
			case MAIN:
				gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
				break;

			case NUM_PLAYERS_SEL:
				previousMenu = MenuState::MAIN;
				createNumPlaySelection(*window, gameDisplay);
				break;

			case TYPE_OF_GAME:
				previousMenu = MenuState::NUM_PLAYERS_SEL;
				createGameTypeMenu(*window, gameDisplay);
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

	void createNumPlaySelection(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);
		GameGUI::HorizontalBoxLayout* hboxQuit = menu->addHorizontalBoxLayout();


		menu->addButton("                2 jugadores               ", ButtonActions::TWOPLAYERS);
		menu->addButton("                3 jugadores                 ", ButtonActions::THREEPLAYERS);
		menu->addButton("                4 jugadores                 ", ButtonActions::FOURPLAYERS);
		//	menu->addButton("                   Atras       ", ButtonActions::BACK);
		hboxQuit->addButton("Atras", ButtonActions::BACK);


		createBackgroundMenu(window);
	}

	void createGameTypeMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay) {

		menu = new GameGUI::Menu(window);
		GameGUI::HorizontalBoxLayout* hboxQuit = menu->addHorizontalBoxLayout();

		menu->addButton("                Todos contra todos              ", ButtonActions::ALLVSALL);
		menu->addButton("                Batalla por equipos                 ", ButtonActions::TEAM);
		//menu->addButton("            		    Atras      ", ButtonActions::BACK);

		hboxQuit->addButton("Atras", ButtonActions::BACK);

		createBackgroundMenu(window);

	}


	void createAllVSAllMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay, int numPlayers) {
		menu = new GameGUI::Menu(window);
		GameGUI::HorizontalBoxLayout* hboxQuit = menu->addHorizontalBoxLayout();
		switch ((numPlayers)) {
		case 2:
			menu->addButton("                   Jugador vs IA               ", ButtonActions::PLVSIA);
			menu->addButton("                  Jugador vs Jugador                ", ButtonActions::PLVSPL);
			hboxQuit->addButton("Atras", ButtonActions::BACK);
			break;

		case 3:
			menu->addButton("               1  Jugador , 2 IA                ", ButtonActions::PL12IA);
			menu->addButton("               2  Jugadores , 1 IA                 ", ButtonActions::PL21IA);
			hboxQuit->addButton("Atras", ButtonActions::BACK);
			break;

		case 4:
			menu->addButton("                 1 Jugador , 3 IA                ", ButtonActions::PL13IA);
			menu->addButton("                2 Jugadores , 2 IA                 ", ButtonActions::PL22IA);
			hboxQuit->addButton("Atras", ButtonActions::BACK);
			break;

		default:
			break;
		}
		createBackgroundMenu(window);
	}



	void createTeamVSTeamMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay, int numPlayers) {
		menu = new GameGUI::Menu(window);
		GameGUI::HorizontalBoxLayout* hboxQuit = menu->addHorizontalBoxLayout();

		switch ((numPlayers)) {

		case 3:
			menu->addButton("                 1 Jugador Vs 2 IA                ", ButtonActions::PL1VS2IA);
			menu->addButton("                 2 Jugadores Vs 1 IA                 ", ButtonActions::PL2VS1IA);
			menu->addButton("                1 Jugador vs Jugador IA                ", ButtonActions::PLVSPLIA);
			hboxQuit->addButton("Atras", ButtonActions::BACK);

			break;

		case 4:
			menu->addButton("           	1 Jugador Vs  3 IA                ", ButtonActions::PL1VS3IA);
			menu->addButton("          	    2 Jugadores Vs 2 IA                ", ButtonActions::PL2VS2IA);
			menu->addButton("             Jugador IA  Vs Jugador IA                 ", ButtonActions::PLIAVSPLIA);
			hboxQuit->addButton("Atras", ButtonActions::BACK);
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
		if (gameDisplay.multiplayerReprocessDisplay) {
			gameDisplay.multiplayerReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow());
	}


};

