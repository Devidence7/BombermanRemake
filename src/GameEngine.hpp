#pragma once
#include <SFML/Graphics.hpp>

#include "Include/global.hpp"
#include "Include/EntitiesInclude.hpp"
#include "Interface/GameDisplayController.h"
#include "Map/Level.hpp"
#include "Textures/TextureStorage.h"
#include "Music/GameMusic.h"
#include "Utils/IAFunctions.hpp"
///#include "Interface/GameDisplayController.h"

#define MAX_NUMBER_OF_STAGES 3
#define DEFAULT_NUM_OF_ENEMIES 10

Level* level;

using namespace sf;

/**
 *
 * Clase encargada de inicialización de niveles.
 * Mostrará al comienzo el menú
 * Mostrará al morir el Game Over
 */

class Game {
private:
	// Dim level map:
	int dimY = 15;
	int dimX = 25;

	double lastCameraMovedTime;

	/*double timeLeft = 299;
	double timeToShow=timeLeft;*/

	// Initialize textures
	TextureStorage textureStorage;
	int numEnemigos;
	//MainMenu mainMenu;

public:

	int stage;
	GameOptions gameOptions;
	bool debug = false;
	bool timesUp = false;
	bool samePlay = false;
	int scoreForTheMoment = 0;

	Game() {
		stage = 1;
		gameOptions.colorList = new sf::Color[10];
		gameOptions.colorList[0] = sf::Color(255, 255, 255, 225);
		gameOptions.colorList[1] = sf::Color(0, 0, 0, 225);
		gameOptions.colorList[2] = sf::Color(255, 0, 0, 225);
		gameOptions.colorList[3] = sf::Color(0, 255, 0, 225);
		gameOptions.colorList[4] = sf::Color(0, 0, 255, 225);
		gameOptions.colorList[5] = sf::Color(150, 0, 150, 225);
		gameOptions.colorList[6] = sf::Color(0, 150, 150, 225);
		gameOptions.colorList[7] = sf::Color(150, 150, 0, 225);
		gameOptions.colorList[8] = sf::Color(100, 150, 40, 225);
		gameOptions.colorList[9] = sf::Color(40, 100, 150, 225);
	}
	int gameTime() {
		return GlobalTime::timeLeft;
	}

	void insertPlayers(UserKeyPress& userKeyPress, int numPlayers, int numIAPlayer) {
		int numPlayersInserter = 0;

		for (int i = 0; i < numPlayers; i++) {
			switch (i) {
			case 0:
				PLayers::addPlayer(userKeyPress.getPlayerControls(i + 1), SIZE_PILLAR, 0, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn(SIZE_PILLAR, 0);
				numPlayersInserter++;
				PLayers::getVectorPlayer()[0]->changeColor(gameOptions.colorList[gameOptions.player1ColorPick]);
				break;

			case 1:
				PLayers::addPlayer(userKeyPress.getPlayerControls(i + 1), SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn(SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR);
				numPlayersInserter++;
				PLayers::getVectorPlayer()[1]->changeColor(gameOptions.colorList[gameOptions.player2ColorPick]);
				break;

			default:
				break;
			}

		}


		for (int i = 0; i < numIAPlayer; i++) {
			switch (i) {

			case 0:
				cout << "IA 1" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), (dimX + 1) * SIZE_PILLAR, 0, debug, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn((dimX)*SIZE_PILLAR, 0);
				numPlayersInserter++;
				break;

			case 1:
				cout << "IA 2" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), (dimX + 1) * SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR, debug, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn((dimX)*SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR);
				numPlayersInserter++;
				break;

			case 2:
				cout << "IA 3" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR, debug, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn(SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR);
				numPlayersInserter++;
				break;

			default:
				break;
			}
		}
	}

	void startNewGame(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		// Restart time parameters
		GameTime::startGameTime();

		//	cout<<gameOptions.numPlayers<<endl;
		int numEnemies = DEFAULT_NUM_OF_ENEMIES * gameOptions.difLevel + (stage / 0.5);
		if (!debug && gameOptions.historyMode) {
			if (!samePlay) {
				stage = 1;
			}
			Enemies::insertarEnemigos(dimX, dimY, numEnemies, stage, gameOptions.difLevel);
		}
		else if (!gameOptions.historyMode) {
			stage = gameOptions.selectedStage;
		}
		//insertEnemies(7);
		if (level != nullptr) {
			delete(level);
		}

		level = new Level(dimX, dimY, debug, stage, &gameOptions);
		PointsDestroyMap::resetMap();
		//Enemies::insertarEnemigos(dimX, dimY);
		if (debug) {
			insertPlayers(*gameDisplay.userKeyPress, 1, 1);
		}
		else {
			cout << "Numero de Players: " << gameOptions.numPlayers << endl;
			cout << "Numero de IAs: " << gameOptions.numIAPlayers << endl;
			insertPlayers(*gameDisplay.userKeyPress, gameOptions.numPlayers, gameOptions.numIAPlayers);
		}

		gameDisplay.notifyChangeDisplay();
		unsigned int pixelsX = window.getSize().x;
		unsigned int pixelsY = window.getSize().y;

		sf::View view(sf::FloatRect(0.f, 0.f, pixelsX, pixelsY));
		view.move(sf::Vector2f(0, -48));
		window.setView(view);

		if (gameOptions.historyMode) {
			if (!samePlay) {
				samePlay = true;
				if (gameOptions.numPlayers > 1) {
					for (auto player : PLayers::getVectorPlayer()) {
						player->lives = 2;
					}
				}
			}
		}

		PLayers::getVectorPlayer()[0]->score = scoreForTheMoment;
		scoreForTheMoment = 0;
	}

	void restartGame(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		timesUp = false;
		
		cout << "Enemigos antes de reiniciar: " << Enemies::getVectorEnemies().size() << endl;
		deleteMap();

		cout << "Enemigos después de reiniciar : " << Enemies::getVectorEnemies().size() << endl;
		//startNewGame(window,gameDisplay);
	}

	void passLevel() {
		if (stage <= MAX_NUMBER_OF_STAGES) {
			stage = stage + 1;
		}
		else {
			//PANTALLA DE VICTORIA FINAL
		}
	}

	void newStage(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		deleteMap();
		stage++;
		Enemies::insertarEnemigos(dimX, dimY, DEFAULT_NUM_OF_ENEMIES * gameOptions.difLevel + (stage / 0.5), stage, gameOptions.difLevel);
		startNewGame(window, gameDisplay);

	}

	void deleteMap() {
		//delete level;
		auto it = Enemies::getVectorEnemies().begin();
		while (it != Enemies::getVectorEnemies().end()) {
			it->reset();
			it = Enemies::getVectorEnemies().erase(it);
		}
		Enemies::getVectorEnemies().clear();
		auto it2 = PLayers::getVectorPlayer().begin();
		while (it2 != PLayers::getVectorPlayer().end()) {
			it2->reset();
			it2 = PLayers::getVectorPlayer().erase(it2);
		}
		PLayers::getVectorPlayer().clear();
	}

	void updatePlayers(GameDisplayController& gameDisplay) {
		int ply = 1;
		for (Player_ptr player : PLayers::getVectorPlayer()) {
			switch (ply) {
			case 1:
				player->updatePlayer();

				break;

			case 2:
				player->updatePlayer();
				//level->checkSpawn(SIZE_PILLAR,(dimY-1)*SIZE_PILLAR);
				break;

			case 3:
				player->updatePlayer();
				//level->checkSpawn((dimX-1)*SIZE_PILLAR,SIZE_PILLAR);
				break;

			case 4:
				player->updatePlayer();
				//level->checkSpawn((dimX-1)*SIZE_PILLAR,(dimY-1)*SIZE_PILLAR);
				break;

			default:
				break;

			}


			//player->updatePlayer();
			player->playerActions();

			level->checkAndFixCollisions(player);
			colissionWithEnemies(player);

			ply++;
		}
	}

	void moveCamera(GameDisplayController& gameDisplay) {
		gameDisplay.updateCamera();
		double moveTime = GameTime::getTimeNow() - lastCameraMovedTime;

		moveTime = fmax(fmin(moveTime, 1), 0);
		moveTime *= 60;

		if (PLayers::getVectorPlayer().size() == 1) {
			sf::Vector2f distCenter2Player = PLayers::getVectorPlayer()[0]->getCenterPosition() - gameDisplay.camera.getCenter();

			sf::Vector2f squareRadius = sf::Vector2f(gameDisplay.getWindow()->getSize().x / 6, gameDisplay.getWindow()->getSize().y / 6);
			if (distCenter2Player.x > squareRadius.x && gameDisplay.camera.getCenter().x + gameDisplay.camera.getSize().x / 2 < level->sizeLevel().x * SIZE_PILLAR + SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x - squareRadius.x) / 20) * moveTime, gameDisplay.camera.getCenter().y));
			}
			else if (distCenter2Player.x < -squareRadius.x && gameDisplay.camera.getCenter().x - gameDisplay.camera.getSize().x / 2 > -SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x + squareRadius.x) / 20) * moveTime, gameDisplay.camera.getCenter().y));
			}

			if (distCenter2Player.y > squareRadius.y && gameDisplay.camera.getCenter().y + gameDisplay.camera.getSize().y / 2 < level->sizeLevel().y * SIZE_PILLAR + SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y - squareRadius.y) / 20) * moveTime));
			}
			else if (distCenter2Player.y < -squareRadius.y && gameDisplay.camera.getCenter().y - gameDisplay.camera.getSize().y / 2 > -SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y + squareRadius.y) / 20) * moveTime));
			}
		}
		else {
			gameDisplay.getWindow()->getSize();

			sf::Vector2f zoomLessRadius = sf::Vector2f(gameDisplay.getWindow()->getSize().x / 6, gameDisplay.getWindow()->getSize().y / 6);
			sf::Vector2f zoomMoreRadius = sf::Vector2f(zoomLessRadius.x * 2, zoomLessRadius.y * 2);
			sf::Vector2f squareRadius = sf::Vector2f(zoomLessRadius.x * 0.5, zoomLessRadius.y * 0.5);
			double zoomValue = gameDisplay.getWindow()->getSize().x / gameDisplay.camera.getSize().x;

			for (Player_ptr player : PLayers::getVectorPlayer()) {
				sf::Vector2f distCenter2Player = sf::Vector2f(gameDisplay.getWindow()->mapCoordsToPixel(player->getCenterPosition()) - gameDisplay.getWindow()->mapCoordsToPixel(gameDisplay.camera.getCenter()));
				//cout << distCenter2Player.x << "   ---   " << distCenter2Player.y << endl;

				sf::Vector2f squareRadius = sf::Vector2f(gameDisplay.getWindow()->getSize().x / 6, gameDisplay.getWindow()->getSize().y / 6);
				if (distCenter2Player.x > squareRadius.x) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x - squareRadius.x) / 20) * moveTime, gameDisplay.camera.getCenter().y));
				}
				else if (distCenter2Player.x < -squareRadius.x) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x + squareRadius.x) / 20) * moveTime, gameDisplay.camera.getCenter().y));
				}

				if (distCenter2Player.y > squareRadius.y) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y - squareRadius.y) / 20) * moveTime));
				}
				else if (distCenter2Player.y < -squareRadius.y) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y + squareRadius.y) / 20) * moveTime));
				}

				if (distCenter2Player.y > zoomMoreRadius.y || distCenter2Player.y < -zoomMoreRadius.y) {
					gameDisplay.camera.zoom(1.0 + 0.005 * moveTime);
				}
				if (distCenter2Player.x > zoomMoreRadius.x || distCenter2Player.x < -zoomMoreRadius.x) {
					gameDisplay.camera.zoom(1.0 + 0.005 * moveTime);
				}
				if (zoomValue < 1.2 && (distCenter2Player.y < zoomLessRadius.y && distCenter2Player.y > -zoomLessRadius.y) && (distCenter2Player.x < zoomLessRadius.x && distCenter2Player.x > -zoomLessRadius.x)) {
					gameDisplay.camera.zoom(1.0 - 0.001 * moveTime);
				}

			}
		}

		lastCameraMovedTime = GameTime::getTimeNow();
		gameDisplay.updateCamera();
	}

	void update(GameDisplayController& gameDisplay) {
		level->update();
		PointsDestroyMap::updateMap();

		if (gameOptions.historyMode) {
			if (level->levelFinished) {
				if (stage < 3) {
					gameDisplay.setGameState(GameDisplayController::VICTORY);
					gameDisplay.notifyChangeDisplay();
					for (auto player : PLayers::getVectorPlayer()) {
						scoreForTheMoment += player->score;
					}
				}
				else {
					gameDisplay.setGameState(GameDisplayController::FINAL_SCORE);
					gameDisplay.newScore = true;
					gameDisplay.notifyChangeDisplay();
				}
			}
		}
		else {
			if (gameOptions.numTeams != 0) {
				bool team1Alive = false;
				bool team2Alive = false;

				for (auto player : PLayers::getVectorPlayer()) {
					if (player->lives >= 1) {
						if (player->team == 0) {
							team1Alive = true;
						}
						else {
							team2Alive = true;
						}
					}
				}
				if (!team1Alive || !team2Alive) {
					gameDisplay.setGameState(GameDisplayController::END_BATTLE);
					gameDisplay.notifyChangeDisplay();
				}
			}
			else {
				int numPlayersAlive = 0;
				for (auto player : PLayers::getVectorPlayer()) {
					if (player->lives >= 1) {
						numPlayersAlive += 1;
					}
				}
				if (numPlayersAlive <= 1) {
					gameDisplay.setGameState(GameDisplayController::END_BATTLE);
					gameDisplay.notifyChangeDisplay();
				}
			}

		}


		updatePlayers(gameDisplay);
		updateEnemies();

		int totalLives = 0;
		for (Player_ptr player : PLayers::getVectorPlayer()) {
			totalLives += player->getLives();
		}

		if (totalLives == 0) {
			samePlay = false;
			gameDisplay.setGameState(GameDisplayController::GameState::GAME_OVER);
		}

		//Update camera:
		moveCamera(gameDisplay);

		GlobalTime::timeLeft = GlobalTime::totalTime - GameTime::getTimeNow();;
		if (GlobalTime::timeLeft <= 0 && !timesUp) {
			timesUp = true;
			//gameDisplay.setGameState(GameDisplayController::GameState::VICTORY);
			Enemies::insertarEnemigosExtra(dimX, dimY, 5);
		}
	}

	void drawPlayers(sf::RenderWindow& w) {

		for (Player_ptr player : PLayers::getVectorPlayer()) {
			player->drawEntity(w);

#ifdef HITBOX_DEBUG_MODE
			PlayerIA_ptr pIA;
			if ((pIA = std::dynamic_pointer_cast<PlayerIAEntity>(player)) != nullptr) {
				pIA->drawMovements(w);
			}
			player->drawEntityHitbox(w);
#endif
		}
	}

	void drawEnemies(sf::RenderWindow& w) {
		for (Enemy_ptr e : Enemies::getVectorEnemies()) {
			e->drawShadow(w);
			e->drawEntity(w);
			
#ifdef HITBOX_DEBUG_MODE
			e->drawEntityHitbox(w);
			//		e->generateMovements();
			e->drawMovements(w);
#endif
		}

		for (Enemy_ptr e2 : Enemies::getVectorEnemiesExtra()) {
			w.draw(*e2);
#ifdef HITBOX_DEBUG_MODE
			e2->drawEntityHitbox(w);
			//		e->generateMovements();
			e2->drawMovements(w);
#endif
		}

	}

	void colissionWithEnemies(Entity_ptr eCol) {
		for (Enemy_ptr e : Enemies::getVectorEnemies()) {
			if (e->CanHurtPlayer() && e->collision(*eCol)) {
				e->onCollission(eCol, e, CollisionType::NONE);
			}
		}
	}



	void updateEnemies() {
		auto it = Enemies::getVectorEnemies().begin();
		int counter = 0;

		while (it != Enemies::getVectorEnemies().end()) {

			// Update the enemies.
			(*it)->update();
			level->checkAndFixCollisions((*it));

			if ((*it)->getExpiredEntity()) {
				it->reset();
				it = Enemies::getVectorEnemies().erase(it);
				numEnemigos--;
				if (numEnemigos == 0) {
					//level->Level::tel.openTeleporter();
				}
			}

			else {
				++it;
				counter++;
			}

		}
		auto it2 = Enemies::getVectorEnemiesExtra().begin();
		counter = 0;
		while (it2 != Enemies::getVectorEnemiesExtra().end()) {

			// Update the enemies.
			(*it2)->update();
			level->checkAndFixCollisions((*it2));

			if ((*it2)->getExpiredEntity()) {
				it2->reset();
				it2 = Enemies::getVectorEnemies().erase(it2);

			}

			else {
				++it2;
				counter++;
			}

		}
	}

	void draw(RenderWindow& w) {
		level->draw(w);
		drawEnemies(w);
		drawPlayers(w);
	}


};
