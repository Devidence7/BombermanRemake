#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"


class PickColorMenu {
	GameGUI::Menu* menu;

	bool EsqPressed = false;

	enum ButtonActions {
		COLOR_1,
		COLOR_2,
		CONTINUE,
		GO_MAIN_MENU
	};

	sf::Texture texture;
	sf::Texture textureBomberman;
	sf::Sprite background;
	sf::Sprite backgroundBomberman;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Slider* color1;
	GameGUI::Slider* color2;

	PlayerTexture* playerTexture;
	PlayerColor* playerColor;

	sf::Sprite playerHeadColor1;
	sf::Sprite playerHead1;

	sf::Sprite playerHeadColor2;
	sf::Sprite playerHead2;

	void createBackgroundMenu(sf::RenderWindow& window, Game& game) {
		menu->setPosition(sf::Vector2f((int)window.getSize().x / 2 - (int)menu->getSize().x / 2, (int)window.getSize().y / 2 - (int)menu->getSize().y / 2));

		playerHeadColor1.setPosition(sf::Vector2f(menu->getPosition().x + 90, menu->getPosition().y+80));
		playerHead1.setPosition(sf::Vector2f(menu->getPosition().x + 90, menu->getPosition().y+80));

		playerHeadColor2.setPosition(sf::Vector2f(menu->getPosition().x + 340, menu->getPosition().y +80));
		playerHead2.setPosition(sf::Vector2f(menu->getPosition().x + 340, menu->getPosition().y+80));

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
	void buildPickUpMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay, Game& game) {
		menu = new GameGUI::Menu(window);

		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);

    	/*textureBomberman.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
		backgroundBomberman.setTexture(textureBomberman);
		float resizeVal = std::fmax((float)window.getSize().x / textureBomberman.getSize().x, (float)window.getSize().y / textureBomberman.getSize().y);
		backgroundBomberman.setScale(resizeVal, resizeVal);
		window.clear(sf::Color::Black);
		window.draw(backgroundBomberman);
		window.display();*/

		background.setColor(sf::Color(255, 255, 0, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ window.getPosition().x, window.getPosition().y, (int)window.getSize().x, (int)window.getSize().y });

		GameGUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();
		GameGUI::VerticalBoxLayout* vbox1 = hbox->addVerticalBoxLayout();
		GameGUI::VerticalBoxLayout* vbox2 = hbox->addVerticalBoxLayout();

		PlayerTexture* playerTexture = &TextureStorage::getPlayerTexture();
		PlayerColor* playerColor = &TextureStorage::getPlayerColor();

		playerHeadColor1.setTexture(playerColor->getTexture());
		playerHeadColor1.setTextureRect(sf::IntRect(sf::Vector2i(9, 0), sf::Vector2i(39, 72)));
		playerHeadColor1.setColor(game.gameOptions.colorList[game.gameOptions.player1ColorPick]);

		playerHead1.setTexture(playerTexture->getTexture());
		playerHead1.setTextureRect(sf::IntRect(sf::Vector2i(9, 0), sf::Vector2i(39, 72)));

		playerHeadColor2.setTexture(playerColor->getTexture());
		playerHeadColor2.setTextureRect(sf::IntRect(sf::Vector2i(9, 0), sf::Vector2i(39, 72)));
		playerHeadColor2.setColor(game.gameOptions.colorList[game.gameOptions.player2ColorPick]);

		playerHead2.setTexture(playerTexture->getTexture());
		playerHead2.setTextureRect(sf::IntRect(sf::Vector2i(9, 0), sf::Vector2i(39, 72)));

		vbox1->add(new Label("   Player 1"));
		vbox1->add(new Label(""));
		vbox1->add(new Label(""));
		vbox1->add(new Label(""));
		vbox1->add(new Label(""));
		vbox1->add(new Label("                      "));
		color1 = new Slider();
		color1->setValue(game.gameOptions.player1ColorPick * 10);
		vbox1->add(color1, ButtonActions::COLOR_1);

		if (game.gameOptions.numPlayers > 1) {
			vbox2->add(new Label("   Player 2"));
			vbox2->add(new Label(""));
			vbox2->add(new Label(""));
			vbox2->add(new Label(""));
			vbox2->add(new Label(""));
			vbox2->add(new Label(""));

			color2 = new Slider();
			color2->setValue(game.gameOptions.player2ColorPick * 10);
			vbox2->add(color2, ButtonActions::COLOR_2);
		}



		menu->addButton("             Continuar            ", ButtonActions::CONTINUE);
		menu->addButton("                Atras                ", ButtonActions::GO_MAIN_MENU);

		createBackgroundMenu(window, game);
	}

	PickColorMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay, Game& game) {
		buildPickUpMenu(window, gameDisplay, game);
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::COLOR_1: {
			cout << color1->getValue() << endl;
			int val = color1->getValue() / 10;
			cout << val << endl;
			game.gameOptions.player1ColorPick = val;
			playerHeadColor1.setColor(game.gameOptions.colorList[game.gameOptions.player1ColorPick]);
			break; }

		case ButtonActions::COLOR_2:{
			int val = color2->getValue() / 10;
			game.gameOptions.player2ColorPick = val;
			playerHeadColor2.setColor(game.gameOptions.colorList[game.gameOptions.player2ColorPick]);
			break;
		}
				
		case ButtonActions::CONTINUE:
			gameDisplay.setGameState(GameDisplayController::GameState::LOADING);
			break;
				
		case ButtonActions::GO_MAIN_MENU:
			gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
			break;
				
		default:
			break;
		}
	}

	void draw(sf::RenderWindow& window, Game& game) {
		// window.draw(backgroundBomberman);
		window.draw(background);

		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow1);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);

		window.draw(*menu);
	
		window.draw(playerHead1);
		window.draw(playerHeadColor1);
		if (game.gameOptions.numPlayers > 1) {
			window.draw(playerHead2);
			window.draw(playerHeadColor2);
		}

		
	}

public:
	void menuActions(GameDisplayController& gameDisplay, Game& game) {
		if (gameDisplay.colorPickerReprocessDisplay) {
			gameDisplay.colorPickerReprocessDisplay = false;
			buildPickUpMenu(*gameDisplay.getWindow(), gameDisplay, game);
		}
		draw(*gameDisplay.getWindow(), game);

		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&PickColorMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));

	}
};

