#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class TutorialInterface {
	GameGUI::Menu* menu;

	bool EsqPressed = false;

	enum ButtonActions {
		CONTINUE,
		QUIT
	};

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

	void centerElement(GameGUI::Widget* e) {
		sf::Vector2f eSize = e->getSize();
		sf::Vector2f mSize = menu->getSize();
		e->setPosition(e->getPosition() + sf::Vector2f(mSize.x / 2 - eSize.x / 2, 0));
	}

sf::Texture powerUpTuto;
sf::Sprite powerUpTutoSprite;

public:
	TutorialInterface(sf::RenderWindow& window, GameDisplayController& gameDisplay) {

		if (!this->powerUpTuto.loadFromFile("../textures/mainMenu/Tutorial_Power_Up.png")) {
			throw ExceptionLoadImage("Imagen no cargada: ../textures/mainMenu/Tutorial_Power_Up.png");
		}

		powerUpTutoSprite.setTexture(powerUpTuto);

		createIntro1(window, gameDisplay);
	}

	int introMenu = 1;

	void createIntro1(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);

		auto title = menu->add(new Label("Bienvenido a Bomberman!"));
		menu->add(new Label(" "));
		auto a1 = menu->add(new Label("Para empezar a jugar es tan sencillo como"));
		auto a2 = menu->add(new Label("utilizar tu boton de bomba: " + gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.UseBomb)));
		auto a3 = menu->add(new Label("Intenta no quemarte despues de hacerlo"));
		menu->add(new Label(" "));
		auto continueButton = menu->addButton("                    Continuar                    ", ButtonActions::CONTINUE);

		createBackgroundMenu(window);

		centerElement(title);
		centerElement(a1);
		centerElement(a2);
		centerElement(a3);
		centerElement(continueButton);
	}

	void createIntro2(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);

		auto title = menu->add(new Label("Destruir muros da reconpensas!"));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->add(new Label(" "));
		menu->addButton("                    Entendido!                    ", ButtonActions::CONTINUE);

		createBackgroundMenu(window);

		centerElement(title);
		powerUpTutoSprite.setScale(0.8,0.8);
		powerUpTutoSprite.setPosition(menu->getPosition().x + 150, menu->getPosition().y + 50);
	}

	void createIntro3(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);

		auto a1 = (menu->add(new Label("Encuentra el portal para")));
		auto a2 = (menu->add(new Label("el siguiente nivel y derrota a ")));
		auto a3 = (menu->add(new Label("todos los enemigos para abrirlo.")));
		menu->add(new Label(" "));
		auto a5 = (menu->addButton("                  Bien...                  ", ButtonActions::CONTINUE));

		createBackgroundMenu(window);
		centerElement(a1);
		centerElement(a2);
		centerElement(a3);
		centerElement(a5);
	}

	void createIntro4(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);

		auto a1 = (menu->add(new Label("Supera los tres niveles para")));
		auto a2 = (menu->add(new Label("poder escribir tu nombre en")));
		auto a3 = (menu->add(new Label("la lista de ganadores.")));
		menu->add(new Label(" "));
		auto a4 = (menu->add(new Label("Buena Suerte!")));
		menu->add(new Label(" "));
		auto a5 = (menu->addButton("            Por favor, callate ya!            ", ButtonActions::QUIT));

		createBackgroundMenu(window);
		centerElement(a1);
		centerElement(a2);
		centerElement(a3);
		centerElement(a4);
		centerElement(a5);
	}


	void createAbility(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);

		auto title = menu->add(new Label("Has conseguido una habilidad!"));
		menu->add(new Label(" "));
		auto a1 = menu->add(new Label("Las habilidades se muestran al lado de"));
		auto a2 = menu->add(new Label("tu personaje en la parte de arriba de la"));
		auto a3 = menu->add(new Label("pantalla."));
		auto a4 = menu->add(new Label("Existen habilidades activas y pasivas."));
		auto a5 = menu->add(new Label("Si es activa utiliza el boton accion " + gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.MakeAction)));
		auto a6 = menu->add(new Label("cerca de una bomba a ver que pasa..."));
		menu->add(new Label(" "));
		auto continueButton = menu->addButton("                    Entendido!                    ", ButtonActions::QUIT);

		createBackgroundMenu(window);

		centerElement(title);
		centerElement(a1);
		centerElement(a2);
		centerElement(a3);
		centerElement(a4);
		centerElement(a5);
		centerElement(a6);
		centerElement(continueButton);
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {				
		case ButtonActions::CONTINUE:
			delete(menu);
			introMenu++;
			if (introMenu == 2) {
				createIntro2(*window, gameDisplay);
			}
			else if (introMenu == 3) {
				createIntro3(*window, gameDisplay);
			}
			else {
				createIntro4(*window, gameDisplay);
			}
			
			break;
				
		case ButtonActions::QUIT:
			gameDisplay.setGameState(GameDisplayController::PLAYING);
			GameTime::resumeGameTime();
			break;
		default:
			break;
		}
		if (id != -1) {
			GameSounds::buttonPress();
		}
	}

	void draw(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		// window.draw(gameDisplay.backgroundBomberman);
		window.draw(gameDisplay.getSquaresBackground());

		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow1);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);

		window.draw(*menu);
		if (introMenu == 2) {
			window.draw(powerUpTutoSprite);
		}

	}

public:
	void menuActions(GameDisplayController& gameDisplay, Game game) {


		if (gameDisplay.tutorialReprocessDisplay) {
			if (gameDisplay.tutorialType == 1) {
				createIntro1(*gameDisplay.getWindow(), gameDisplay);
			}
			else {
				createAbility(*gameDisplay.getWindow(), gameDisplay);
			}
			gameDisplay.tutorialReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow());
		}
		draw(*gameDisplay.getWindow(), gameDisplay);

		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&TutorialInterface::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
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
						gameDisplay.backgroundOpacity = 0;
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

