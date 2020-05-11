#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "OptionsMenu.h"
#include "GameDisplayController.h"
#include <map>


class FinalScoreInterface {
	GameGUI::Menu* menu;
	GameDisplayController::GameState lastGameState;
	bool EsqPressed = false;
	sf::Font font;
	const string SCORE_FILENAME = "scores.txt";
	std::map<int, string> scores;

	enum ButtonActions {
		TEXT,
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
	GameGUI::InputBox* textbox;

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

	void savePuntuation() {
		int i = 0;
		fstream propertiesFile;
		propertiesFile.open(SCORE_FILENAME, std::fstream::in | std::fstream::out | std::fstream::trunc);

		int score = 0;
		for (Player_ptr p : PLayers::getVectorPlayer()) {
			score += p->score;
		}

		scores[score] = textbox->getText();

		for (auto it = scores.rbegin(); it != scores.rend(); it++) {
			propertiesFile << it->first << " " << it->second << "\n";

			if (i >= 5) {
				break;
			}
		}
	}

	void ReadPuntuation() {
		fstream propertiesFile;

		propertiesFile.open(SCORE_FILENAME, std::fstream::in);

		// If file does not exist, Create new file
		if (!propertiesFile.is_open()) {
			propertiesFile.close();
			
		}
		else {
			propertiesFile.close();
		}

		propertiesFile.open(SCORE_FILENAME, std::fstream::in | std::fstream::out);

		for (int i = 0; i < 5; i++) {
			string name;
			int score;
			propertiesFile >> score;
			propertiesFile >> name;
			scores[score] = name;
		}

		for (auto it = scores.rbegin(); it != scores.rend(); it++) {
			std::cout << it->first  // string (key)
				<< ':'
				<< it->second   // string's value 
				<< std::endl;
		}
	}

public:
	FinalScoreInterface(sf::RenderWindow& window, Game& game, GameDisplayController& gameDisplay) {
		createFinalScoreInterface(window, game, gameDisplay);
	}

	void createFinalScoreInterface(sf::RenderWindow& window, Game& game, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);
		if (!font.loadFromFile("../textures/mainMenu/PixelEmulator.ttf")) {
			//cosas
		}
		victory.setFont(font);
		victory.setString("PUNTUACIONES");
		victory.setFillColor(sf::Color::White);
		// victory.setScale(2,2);
		victory.setCharacterSize(72);

		sf::FloatRect textRect = victory.getLocalBounds();
		victory.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		victory.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 12.0f));


		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);
		background.setColor(sf::Color(0, 10, 153, 5));
		background.setTexture(texture);
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });

		auto f = menu->addFormLayout();
		int score = 0;
		for (Player_ptr p : PLayers::getVectorPlayer()) {
			score += p->score;
		}

		Label* yourScore = new GameGUI::Label(to_string(score));
		f->addRow(" >> Tu puntuacion ", yourScore);

		// Textbox
		textbox = new GameGUI::InputBox();
		textbox->setText("Player");
		f->addRow(" >> Introduce Tu Nombre ", textbox, ButtonActions::TEXT);

		ReadPuntuation();
		
		int i = 1;
		for (auto it = scores.rbegin(); it != scores.rend(); it++) {
			GameGUI::Label* l1 = new GameGUI::Label(to_string(it->first));
			f->addRow(to_string(i) + ". " + it->second, l1);
			i++;
		}

		menu->addButton("       Ir al menu principal       ", ButtonActions::GO_MAIN_MENU);
		menu->addButton("                   Salir                    ", ButtonActions::QUIT);

		createBackgroundMenu(window);
	}

private:
	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::GO_MAIN_MENU:
			savePuntuation();
			game.deleteMap();
			gameDisplay.setGameState(GameDisplayController::GameState::MAIN_MENU);
			break;

		case ButtonActions::QUIT:
			savePuntuation();
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
		
		window.draw(victory);
	}

public:
	void menuActions(GameDisplayController& gameDisplay, Game& game) {
		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&FinalScoreInterface::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
		if (gameDisplay.scoreReprocessDisplay) {
			gameDisplay.scoreReprocessDisplay = false;
			createFinalScoreInterface(*gameDisplay.getWindow(), game, gameDisplay);
		}
		draw(*gameDisplay.getWindow());
	}

	void updateLevel(Game& game) {
		level.setString("NIVEL " + to_string(game.stage) + ": Completado");
	}
};

