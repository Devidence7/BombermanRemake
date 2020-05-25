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
#define DEFAULT_NUM_OF_ENEMIES 7

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

	// For story mode when pass to next level.
	bool samePlay = false;

	int scoreForTheMoment = 0;

	double player1_speedBoost = 1;
	int player1_numOfBombs = 1;
	int player1_powerOfBombs = 1;
	int player1_lives = 1;
	ActionsAvalible player1_action = ActionsAvalible::NONE_ACTION;

	double player2_speedBoost = 1;
	int player2_numOfBombs = 1;
	int player2_powerOfBombs = 1;
	int player2_lives = 1;
	ActionsAvalible player2_action = ActionsAvalible::NONE_ACTION;


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

	int getRandomIA(int difficulty) {
		return Random::getIntNumberBetween(1,3) + difficulty * 3;
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
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), (dimX + 1) * SIZE_PILLAR, 0, debug, gameOptions.playersAndTeams[numPlayersInserter], getRandomIA(gameOptions.IADifficulty));
				level->checkSpawn((dimX)*SIZE_PILLAR, 0);
				numPlayersInserter++;
				break;

			case 1:
				cout << "IA 2" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), (dimX + 1) * SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR, debug, gameOptions.playersAndTeams[numPlayersInserter], getRandomIA(gameOptions.IADifficulty));
				level->checkSpawn((dimX)*SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR);
				numPlayersInserter++;
				break;

			case 2:
				cout << "IA 3" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR, debug, gameOptions.playersAndTeams[numPlayersInserter], getRandomIA(gameOptions.IADifficulty));
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

		timesUp = false;

		//	cout<<gameOptions.numPlayers<<endl;
		int numEnemies = DEFAULT_NUM_OF_ENEMIES * gameOptions.difLevel * ((float)stage * 0.2 + 1);
		GlobalTime::totalTime = 299;
		if (!debug && gameOptions.historyMode) {
			if (!samePlay) {
				stage = 1;
			}
			Enemies::insertarEnemigos(dimX, dimY, numEnemies, stage, gameOptions.difLevel);
			if (gameOptions.difLevel <= 1.250) {
				GlobalTime::totalTime = 299;
			}
			else if (gameOptions.difLevel <= 1.500) {
				GlobalTime::totalTime = 239;
			}
			else {
				GlobalTime::totalTime = 209;
			}

		}
		else if (!gameOptions.historyMode) {
			stage = gameOptions.selectedStage + 1;
		}
		
		if (level != nullptr) {
			delete(level);
		}

		if (gameOptions.historyMode) {
			dimX = 25;
			dimY = 15;
			level = new Level(dimX, dimY, debug, stage, &gameOptions);
		}
		else {
			// Small
			if (gameOptions.selectedSizeStage == 0) {
				dimX = 11;
				dimY = 11;
				level = new Level(dimX, dimY, debug, stage, &gameOptions);
			}
			// Medium
			else if (gameOptions.selectedSizeStage == 1) {
				dimX = 21;
				dimY = 11;
				level = new Level(dimX, dimY, debug, stage, &gameOptions);
			}
			// Big
			else {
				dimX = 25;
				dimY = 15;
				level = new Level(dimX, dimY, debug, stage, &gameOptions);
			}
		}
		


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
		
		gameDisplay.updateCamera();
		gameDisplay.camera.setCenter(level->sizeLevel().x * SIZE_PILLAR / 2, level->sizeLevel().y * SIZE_PILLAR / 2);
	

		if (gameOptions.historyMode) {
			if (!samePlay) {
				samePlay = true;
				if (gameOptions.numPlayers > 1) {
					for (auto player : PLayers::getVectorPlayer()) {
						player->lives = 2;
					}
				}
			}
			else {
				PLayers::getVectorPlayer()[0]->setStats(player1_speedBoost, player1_numOfBombs, player1_powerOfBombs, player1_lives, player1_action);
				if (PLayers::getVectorPlayer().size() > 1) {
					PLayers::getVectorPlayer()[1]->setStats(player2_speedBoost, player2_numOfBombs, player2_powerOfBombs, player2_lives, player2_action);
				}
			}

			PLayers::getVectorPlayer()[0]->storyMode = true;
			PLayers::getVectorPlayer()[0]->score = scoreForTheMoment;
			if (PLayers::getVectorPlayer().size() > 1) {
				PLayers::getVectorPlayer()[1]->storyMode = true;
			}
		}
		else {
			for (auto player : PLayers::getVectorPlayer()) {
				player->setStats(1 + gameOptions.startingSpeed * 0.2, gameOptions.startingBombs, gameOptions.startingFire, gameOptions.startingLives, gameOptions.startingAbility);
			}

			GlobalTime::totalTime = gameOptions.totalTime - 1;
		}
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
			sf::Vector2f cameraRealCenter = (gameDisplay.camera.getCenter() + sf::Vector2f(0, gameDisplay.camera.getSize().y / 30.0));
			sf::Vector2f distCenter2Player = PLayers::getVectorPlayer()[0]->getCenterPosition() - cameraRealCenter;

			sf::Vector2f squareRadius = sf::Vector2f(gameDisplay.getWindow()->getSize().x / 6, gameDisplay.getWindow()->getSize().y / 6);
			if (distCenter2Player.x > squareRadius.x && gameDisplay.camera.getCenter().x + gameDisplay.camera.getSize().x / 2 < (level->sizeLevel().x + 2) * SIZE_PILLAR + SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x - squareRadius.x) / 20) * moveTime, gameDisplay.camera.getCenter().y));
			}
			else if (distCenter2Player.x < -squareRadius.x && gameDisplay.camera.getCenter().x - gameDisplay.camera.getSize().x / 2 > -SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x + squareRadius.x) / 20) * moveTime, gameDisplay.camera.getCenter().y));
			}

			if (distCenter2Player.y > squareRadius.y && cameraRealCenter.y + gameDisplay.camera.getSize().y / 2 < (level->sizeLevel().y + 2) * SIZE_PILLAR + SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y - squareRadius.y) / 20) * moveTime));
			}
			else if (distCenter2Player.y < -squareRadius.y && cameraRealCenter.y - gameDisplay.camera.getSize().y / 2 > -SIZE_PILLAR * 2) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y + squareRadius.y) / 20) * moveTime));
			}
		}
		else {
			gameDisplay.getWindow()->getSize();

			sf::Vector2f zoomLessRadius = sf::Vector2f(gameDisplay.getWindow()->getSize().x / 6, gameDisplay.getWindow()->getSize().y / 6);
			sf::Vector2f zoomMoreRadius = sf::Vector2f(zoomLessRadius.x * 2, zoomLessRadius.y * 2);
			sf::Vector2f squareRadius = sf::Vector2f(zoomLessRadius.x * 0.5, zoomLessRadius.y * 0.5);
			double zoomValue = gameDisplay.getWindow()->getSize().x / gameDisplay.camera.getSize().x;

			int numPlayersCenter = 0;
			int numPlayersTotal = PLayers::getVectorPlayer().size();

			sf::Vector2f cameraRealCenter = (gameDisplay.camera.getCenter() + sf::Vector2f(0, gameDisplay.camera.getSize().y / 10.0));
			Vector2i camCenter = gameDisplay.getWindow()->mapCoordsToPixel(cameraRealCenter);
			float dimXCam = gameDisplay.camera.getSize().x / 2;
			float dimYCam = gameDisplay.camera.getSize().y / 2;

			// If all map is visible
			if (camCenter.x - dimXCam < 0 &&
				camCenter.x + dimXCam > (level->sizeLevel().x) * SIZE_PILLAR &&
				camCenter.y - dimYCam < 0 &&
				camCenter.y + dimYCam > (level->sizeLevel().y) * SIZE_PILLAR) {
				gameDisplay.camera.move(((level->sizeLevel().x) * SIZE_PILLAR / 2 - cameraRealCenter.x) / 20 * moveTime, ((level->sizeLevel().y + 2) * SIZE_PILLAR / 2 - cameraRealCenter.y) / 20 * moveTime);
				//gameDisplay.camera.setCenter((level->sizeLevel().x) * SIZE_PILLAR / 2 , (level->sizeLevel().y + 2) * SIZE_PILLAR / 2);

				if (camCenter.x - dimXCam < -48 &&
					camCenter.x + dimXCam > (level->sizeLevel().x) * SIZE_PILLAR + 48 &&
					camCenter.y - dimYCam < -48 &&
					camCenter.y + dimYCam > (level->sizeLevel().y) * SIZE_PILLAR + 48) {
					gameDisplay.camera.zoom(1.0 - 0.005 * moveTime);
				}
				for (Player_ptr player : PLayers::getVectorPlayer()) {
					sf::Vector2f distCenter2Player = sf::Vector2f(gameDisplay.getWindow()->mapCoordsToPixel(player->getCenterPosition()) - gameDisplay.getWindow()->mapCoordsToPixel(gameDisplay.camera.getCenter()));
					if (zoomValue < 1.2 && (distCenter2Player.y < zoomLessRadius.y && distCenter2Player.y > -zoomLessRadius.y) && (distCenter2Player.x < zoomLessRadius.x && distCenter2Player.x > -zoomLessRadius.x)) {
						numPlayersCenter++;
					}
				}
				if (numPlayersCenter >= numPlayersTotal) {
					// Zoom in
					gameDisplay.camera.zoom(1.0 - 0.004 * moveTime);
				}
			}
			// If not all map is visible
			else {
				for (Player_ptr player : PLayers::getVectorPlayer()) {
					sf::Vector2f distCenter2Player = sf::Vector2f(gameDisplay.getWindow()->mapCoordsToPixel(player->getCenterPosition()) - gameDisplay.getWindow()->mapCoordsToPixel(gameDisplay.camera.getCenter()));
					//cout << distCenter2Player.x << "   ---   " << distCenter2Player.y << endl;

					if (distCenter2Player.x > squareRadius.x) {
						gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x - squareRadius.x) / 100) * moveTime, gameDisplay.camera.getCenter().y));
					}
					else if (distCenter2Player.x < -squareRadius.x) {
						gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + ((distCenter2Player.x + squareRadius.x) / 100) * moveTime, gameDisplay.camera.getCenter().y));
					}

					if (distCenter2Player.y > squareRadius.y) {
						gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y - squareRadius.y) / 100) * moveTime));
					}
					else if (distCenter2Player.y < -squareRadius.y) {
						gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + ((distCenter2Player.y + squareRadius.y) / 100) * moveTime));
					}



					if (distCenter2Player.y > zoomMoreRadius.y || distCenter2Player.y < -zoomMoreRadius.y) {
						gameDisplay.camera.zoom(1.0 + 0.005 * moveTime);
					}
					if (distCenter2Player.x > zoomMoreRadius.x || distCenter2Player.x < -zoomMoreRadius.x) {
						gameDisplay.camera.zoom(1.0 + 0.005 * moveTime);
					}

					if (player->dead || player->respawning) {
						numPlayersTotal--;
					}
					else {
						if (zoomValue < 1.2 && (distCenter2Player.y < zoomLessRadius.y && distCenter2Player.y > -zoomLessRadius.y) && (distCenter2Player.x < zoomLessRadius.x && distCenter2Player.x > -zoomLessRadius.x)) {
							numPlayersCenter++;
						}
					}
				}
			}

			// If all the players are in the center zoom in
			if (numPlayersCenter >= numPlayersTotal) {
				// Zoom in
				for (Player_ptr player : PLayers::getVectorPlayer()) {
					gameDisplay.camera.zoom(1.0 - 0.001 * moveTime);
				}
			}
		}

		lastCameraMovedTime = GameTime::getTimeNow();
		gameDisplay.updateCamera();
	}

	void update(GameDisplayController& gameDisplay) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//											  BackDoor
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if(!gameOptions.historyMode){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
					PLayers::getVectorPlayer()[0]->lastInvencibleTime = -6;
					PLayers::getVectorPlayer()[0]->lives = 0;
					PLayers::getVectorPlayer()[0]->setExpiredEntity();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && PLayers::getVectorPlayer().size() > 1) {
					PLayers::getVectorPlayer()[1]->lastInvencibleTime = -6;
					PLayers::getVectorPlayer()[1]->lives = 0;
					PLayers::getVectorPlayer()[1]->setExpiredEntity();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && PLayers::getVectorPlayer().size() > 2) {
					PLayers::getVectorPlayer()[2]->lastInvencibleTime = -6;
					PLayers::getVectorPlayer()[2]->lives = 0;
					PLayers::getVectorPlayer()[2]->setExpiredEntity();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && PLayers::getVectorPlayer().size() > 3) {
					PLayers::getVectorPlayer()[3]->lastInvencibleTime = -6;
					PLayers::getVectorPlayer()[3]->lives = 0;
					PLayers::getVectorPlayer()[3]->setExpiredEntity();
				}
			}
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
					PLayers::getVectorPlayer()[0]->lastInvencibleTime = -6;
					PLayers::getVectorPlayer()[0]->lives = 0;
					PLayers::getVectorPlayer()[0]->setExpiredEntity();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && PLayers::getVectorPlayer().size() > 1) {
					PLayers::getVectorPlayer()[1]->lastInvencibleTime = -6;
					PLayers::getVectorPlayer()[1]->lives = 0;
					PLayers::getVectorPlayer()[1]->setExpiredEntity();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
					for (auto enemy : Enemies::getVectorEnemies()) {
						enemy->setExpiredEntity();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
					level->numWalls = 0;
				}
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (gameOptions.historyMode) {
			if (gameDisplay.tutorialIntro) {
				gameDisplay.tutorialType = 1;
				gameDisplay.setGameState(GameDisplayController::TUTORIAL);
				GameTime::stopGameTime();
				gameDisplay.tutorialIntro = false;
				gameDisplay.notifyChangeDisplay();
				gameDisplay.saveProperties();
			}
			else if (gameDisplay.tutorialAbility) {
				if (PLayers::getVectorPlayer()[0]->getActionsAvaliable() != NONE_ACTION) {
					gameDisplay.backgroundOpacity = 0;
					gameDisplay.tutorialType = 2;
					gameDisplay.setGameState(GameDisplayController::TUTORIAL);
					GameTime::stopGameTime();
					gameDisplay.tutorialAbility = false;
					gameDisplay.notifyChangeDisplay();
					gameDisplay.saveProperties();
				}
			}
		}

		level->update();
		PointsDestroyMap::updateMap();

		if (gameOptions.historyMode) {
			if (level->levelFinished) {
				if (stage < 3) {
					gameDisplay.setGameState(GameDisplayController::VICTORY);
					gameDisplay.backgroundOpacity = 0;
					gameDisplay.notifyChangeDisplay();
					int i = 0;
					for (auto player : PLayers::getVectorPlayer()) {
						scoreForTheMoment += player->score;
						if (i == 0) {
							player1_lives = player->lives;
							player1_numOfBombs = player->numOfBombs;
							player1_powerOfBombs = player->powerOfBombs;
							player1_speedBoost = player->speedBoost;
							player1_action = player->getActionsAvaliable();
						}
						else {
							player2_lives = player->lives;
							player2_numOfBombs = player->numOfBombs;
							player2_powerOfBombs = player->powerOfBombs;
							player2_speedBoost = player->speedBoost;
							player2_action = player->getActionsAvaliable();
						}
						i++;
					}
				}
				else {
					gameDisplay.setGameState(GameDisplayController::FINAL_SCORE);
					gameDisplay.backgroundOpacity = 0;
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

			if (timesUp) {
				gameDisplay.setGameState(GameDisplayController::END_BATTLE);
				gameDisplay.notifyChangeDisplay();
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
			e->drawEntity(w);
			
#ifdef HITBOX_DEBUG_MODE
			e->drawEntityHitbox(w);
			//		e->generateMovements();
			e->drawMovements(w);
#endif
		}

//		for (Enemy_ptr e2 : Enemies::getVectorEnemiesExtra()) {
//			w.draw(*e2);
//#ifdef HITBOX_DEBUG_MODE
//			e2->drawEntityHitbox(w);
//			//		e->generateMovements();
//			e2->drawMovements(w);
//#endif
//		}

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
		level->drawShadows(w);

		for (Enemy_ptr e : Enemies::getVectorEnemies()) {
			e->drawShadow(w);
		}

		for (Player_ptr player : PLayers::getVectorPlayer()) {
			player->drawShadow(w);
		}

		level->draw(w);
		drawEnemies(w);
		drawPlayers(w);
		level->drawBombs(w);
	}


};
