#pragma once
#include <SFML/Graphics.hpp>
#include "GameDisplayController.h"
#include "../GameEngine.hpp"

#define MAX_NUMBER_OF_ITEMS 4
#define MAX_NUMBER_OF_PLAYERS 4
#define MAX_NUMBER_OF_PU 4
#define HEAD_DIM 36
#define HEAD_START 10

using namespace std;

class GameInterface {
	sf::RectangleShape container;
	sf::RectangleShape containerBorder;
	sf::Font font;

	sf::Text lives[MAX_NUMBER_OF_PLAYERS];


	sf::RectangleShape timeBox;
	sf::RectangleShape timeBoxBorder;
	sf::Text time;
	sf::RectangleShape scoreBox;
	sf::RectangleShape scoreBoxBorder;
	sf::Text scoreText;

/*	int timeAMostrar;
	int timeLeft = 299;*/
	int dimY = 15;
	int dimX = 25;

public:
	GameInterface(sf::RenderWindow& window) {}

	void iniPlayers(sf::RenderWindow& window, GameDisplayController& gameDisplay, Game& gameEngine) {
		cout << PLayers::getVectorPlayer().size() << endl;
		int score = 0;
		for (int i = 0; i < PLayers::getVectorPlayer().size(); i++) {
			//PLayers::getVectorPlayer()[i]->playerHead2.setPosition(container.getPosition().x + ((container.getSize().x / 4) * i), (container.getPosition().y + 7.5));
			//PLayers::getVectorPlayer()[i]->playerHead.setPosition(container.getPosition().x + ((container.getSize().x / 4) * i), (container.getPosition().y + 7.5));

			float playerHeadPos;

			if (i < 2) {
				playerHeadPos = (container.getSize().x / 2 - timeBoxBorder.getSize().x / 2) / 3;
				playerHeadPos *= (i + 1);
			}
			else {
				playerHeadPos = (container.getSize().x / 2 - timeBoxBorder.getSize().x / 2) / 3;
				playerHeadPos *= (i - 1);
				playerHeadPos += container.getSize().x / 2 + timeBoxBorder.getSize().x / 2;
			}

			PLayers::getVectorPlayer()[i]->playerHead2.setPosition(container.getPosition().x + (playerHeadPos - PLayers::getVectorPlayer()[i]->playerHead2.getTextureRect().width/2),
				(container.getSize().y / 2 - PLayers::getVectorPlayer()[i]->playerHead2.getTextureRect().height / 2));
			PLayers::getVectorPlayer()[i]->playerHead.setPosition(container.getPosition().x + (playerHeadPos - PLayers::getVectorPlayer()[i]->playerHead2.getTextureRect().width / 2),
				(container.getSize().y / 2 - PLayers::getVectorPlayer()[i]->playerHead.getTextureRect().height / 2));

			lives[i].setFont(font);
			// lives[i].setFillColor(sf::Color::White);
			lives[i].setFillColor(sf::Color::White);
			lives[i].setString("X" + to_string(PLayers::getVectorPlayer()[i]->getLives()));
			// lives[i].setPosition(PLayers::getVectorPlayer()[i]->playerHead2.getPosition().x + 35, PLayers::getVectorPlayer()[i]->playerHead2.getPosition().y + 7.5);
			// lives[i].setScale(0.5, 0.5);
			lives[i].setCharacterSize(23);
			//lives[i].setOutlineColor(sf::Color(0, 0, 0));
			//lives[i].setOutlineThickness(3);

			

			sf::FloatRect textRect = lives[i].getLocalBounds();
			lives[i].setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
			// lives[i].setPosition(sf::Vector2f(container.getSize().x / 2.0f, container.getSize().y / 2.0f));

			lives[i].setPosition(PLayers::getVectorPlayer()[i]->playerHead.getPosition().x + PLayers::getVectorPlayer()[i]->playerHead2.getTextureRect().width, //  + PLayers::getVectorPlayer()[i]->playerHead2.getTextureRect().width - lives[i].getLocalBounds().width/2
				PLayers::getVectorPlayer()[i]->playerHead.getPosition().y + PLayers::getVectorPlayer()[i]->playerHead2.getTextureRect().height);

			score += PLayers::getVectorPlayer()[i]->score;
		}

		if (gameEngine.gameOptions.historyMode) {
			scoreText.setFont(font);
			int timeToShow = GlobalTime::timeLeft;
			scoreText.setString("000000");
			//time.setFillColor(sf::Color::White);
			scoreText.setFillColor(sf::Color::White);


			//time.setPosition((container.getSize().x/2) - (time.getLocalBounds().width/2), (container.getSize().y/2) - (time.getLocalBounds().height / 2));
			sf::FloatRect textRect = scoreText.getLocalBounds();
			scoreText.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
			scoreText.setPosition(sf::Vector2f(container.getSize().x * 3 / 4.0f, container.getSize().y * 3 / 4.0f));


			scoreBox.setSize(sf::Vector2f(scoreText.getLocalBounds().width * 1.5, scoreText.getLocalBounds().height * 1.5));
			scoreBox.setFillColor(sf::Color(0, 0, 0));
			scoreBox.setPosition(scoreText.getPosition().x - scoreText.getLocalBounds().width / 2 * 1.5, scoreText.getPosition().y - scoreText.getLocalBounds().height / 2 * 1.5);

			scoreBoxBorder.setSize(sf::Vector2f(time.getLocalBounds().width * 1.5 + 4, time.getLocalBounds().height * 1.5 + 4));
			scoreBoxBorder.setFillColor(sf::Color(255, 0, 0));
			scoreBoxBorder.setPosition(scoreText.getPosition().x - scoreText.getLocalBounds().width / 2 * 1.5 - 2, scoreText.getPosition().y - scoreText.getLocalBounds().height / 2 * 1.5 - 2);
		}


	}

	void createInterface(sf::RenderWindow& window, GameDisplayController &gameDisplay, Game& game){
			this->container.setSize(sf::Vector2f(window.getSize().x, window.getSize().y / 10));
		this->container.setFillColor(sf::Color(255, 170, 0));
		this->container.setPosition(0, 0);

		containerBorder.setSize(sf::Vector2f(container.getSize().x, container.getSize().y + 3));
		containerBorder.setFillColor(sf::Color(0, 0, 0));
		containerBorder.setPosition(0, 0);

		if (!font.loadFromFile("../textures/mainMenu/PixelEmulator.ttf")) {

		}

		time.setFont(font);
		int timeToShow=GlobalTime::timeLeft;
		time.setString(to_string(timeToShow / 60) + ":" + to_string(timeToShow % 60));
		//time.setFillColor(sf::Color::White);
		time.setFillColor(sf::Color::White);


		//time.setPosition((container.getSize().x/2) - (time.getLocalBounds().width/2), (container.getSize().y/2) - (time.getLocalBounds().height / 2));
		sf::FloatRect textRect = time.getLocalBounds();
		time.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		time.setPosition(sf::Vector2f(container.getSize().x / 2.0f, container.getSize().y / 2.0f));


		timeBox.setSize(sf::Vector2f(time.getLocalBounds().width * 1.5, time.getLocalBounds().height * 1.5));
		timeBox.setFillColor(sf::Color(0, 0, 0));
		timeBox.setPosition(time.getPosition().x - time.getLocalBounds().width/2 * 1.5, time.getPosition().y - time.getLocalBounds().height/2 * 1.5);

		timeBoxBorder.setSize(sf::Vector2f(time.getLocalBounds().width * 1.5 + 4, time.getLocalBounds().height * 1.5 + 4));
		timeBoxBorder.setFillColor(sf::Color(255, 0, 0));
		timeBoxBorder.setPosition(time.getPosition().x - time.getLocalBounds().width / 2 * 1.5 - 2, time.getPosition().y - time.getLocalBounds().height / 2 * 1.5 - 2);
		iniPlayers(window, gameDisplay, game);
	}




	void draw(GameDisplayController& gameDisplay, Game& game) {
		if (gameDisplay.gameInterfaceReprocessDisplay) {
			gameDisplay.gameInterfaceReprocessDisplay = false;
			gameDisplay.getWindow()->setView(gameDisplay.playingGuiView);
			createInterface(*gameDisplay.getWindow(), gameDisplay, game);
			gameDisplay.getWindow()->setView(gameDisplay.camera);
		}

		gameDisplay.getWindow()->setView(gameDisplay.playingGuiView);

		gameDisplay.getWindow()->draw(containerBorder);
		gameDisplay.getWindow()->draw(container);

		for (int i = 0; i < PLayers::getVectorPlayer().size(); i++) {
			if (PLayers::getVectorPlayer()[i]->getActionsAvaliable()) {

				PowerUpTexture t = TextureStorage::getPowerUpTexture();
				PLayers::getVectorPlayer()[i]->playerCurrentAbility.setTexture(t.getTexture());
				PLayers::getVectorPlayer()[i]->playerCurrentAbility.setTextureRect(t.getFrame(PLayers::getVectorPlayer()[i]->getActionsAvaliable()));

				PLayers::getVectorPlayer()[i]->playerCurrentAbility.setPosition(
					PLayers::getVectorPlayer()[i]->playerHead2.getPosition().x - PLayers::getVectorPlayer()[i]->playerCurrentAbility.getTextureRect().width / 3 * 2,
					PLayers::getVectorPlayer()[i]->playerHead2.getPosition().y - PLayers::getVectorPlayer()[i]->playerCurrentAbility.getTextureRect().height / 3);
				gameDisplay.getWindow()->draw(PLayers::getVectorPlayer()[i]->playerCurrentAbility);
			}

			gameDisplay.getWindow()->draw(PLayers::getVectorPlayer()[i]->playerHead2);
			gameDisplay.getWindow()->draw(PLayers::getVectorPlayer()[i]->playerHead);

			gameDisplay.getWindow()->draw(lives[i]);
		}

		gameDisplay.getWindow()->draw(timeBoxBorder);
		gameDisplay.getWindow()->draw(timeBox);
		gameDisplay.getWindow()->draw(time);
		

		if (game.gameOptions.historyMode) {
			gameDisplay.getWindow()->draw(scoreBoxBorder);
			gameDisplay.getWindow()->draw(scoreBox);
			gameDisplay.getWindow()->draw(scoreText);
		}

	}


	void update() {
		int i = 0;
		int score = 0;
		for (Player_ptr& player : PLayers::getVectorPlayer()) {
			lives[i].setString("X" + to_string(player->getLives()));
			score += player->score;
			i++;
		}

		scoreText.setString(to_string(score));

		GlobalTime::timeLeft = GlobalTime::totalTime - GameTime::getTimeNow();
		if (GlobalTime::timeLeft < 0) {
			GlobalTime::timeLeft = 0;
		}
		int timeToShow=GlobalTime::timeLeft;
		string seconds =  timeToShow % 60 > 9 ? to_string(timeToShow % 60) : "0" + to_string(timeToShow % 60);
		time.setString(to_string(timeToShow / 60) + ":" + seconds);
	}




};
