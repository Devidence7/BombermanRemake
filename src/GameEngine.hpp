#pragma once
#include <SFML/Graphics.hpp>

#include "Include/global.hpp"
#include "Include/EntitiesInclude.hpp"

#include "Map/Level.hpp"
#include "Textures/TextureStorage.h"
#include "Interface/GameInterface.h"
#include "Music/GameMusic.h"
#include "Utils/IAFunctions.hpp"
///#include "Interface/GameDisplayController.h"

#define MAX_NUMBER_OF_STAGES 3
#define DEFAULT_NUM_OF_ENEMIES 10

Level *level;

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
	
	/*double timeLeft = 299;
	double timeToShow=timeLeft;*/
	
	// Initialize textures
	TextureStorage textureStorage;
	int numEnemigos;
	//MainMenu mainMenu;

public:
	
	int stage;
	GameOptions gameOptions;
	bool debug=false;
	bool timesUp=false;

Game(){
	stage=1;
}
	int gameTime(){
		return GlobalTime::timeLeft;
	}

	void insertPlayers(UserKeyPress &userKeyPress, int numPlayers, int numIAPlayer) {
		int numPlayersInserter = 0;

		for (int i = 0; i < numPlayers; i++) {
			switch (i) {
			case 0:
				PLayers::addPlayer(userKeyPress.getPlayerControls(i + 1), SIZE_PILLAR, 0, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn(SIZE_PILLAR, 0);
				numPlayersInserter++;
				break;

			case 1:
				PLayers::addPlayer(userKeyPress.getPlayerControls(i + 1), SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn(SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR);
				numPlayersInserter++;
				break;

			default:
				break;
			}
			
		}
		for (int i = 0; i < numIAPlayer; i++) {
			switch (i) {

			case 0:
				cout << "IA 1" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), (dimX+1) * SIZE_PILLAR, 0, debug, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn((dimX) * SIZE_PILLAR, 0);
				numPlayersInserter++;
				break;

			case 1:
				cout << "IA 2" << endl;
				PLayers::addIAPlayer(userKeyPress.getPlayerControls(i + 1), (dimX+1) * SIZE_PILLAR, (dimY - 1)*SIZE_PILLAR, debug, gameOptions.playersAndTeams[numPlayersInserter]);
				level->checkSpawn((dimX) * SIZE_PILLAR, (dimY - 1) * SIZE_PILLAR);
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

	void startNewGame(sf::RenderWindow& window, GameDisplayController &gameDisplay){
		// Restart time parameters
		GameTime::startGameTime();

		//	cout<<gameOptions.numPlayers<<endl;
		int numEnemies=DEFAULT_NUM_OF_ENEMIES*gameOptions.difLevel+(stage/0.5);
		cout<<"NumEnemies: "<<numEnemies<<endl;
	
		if(!debug && gameOptions.historyMode){
			Enemies::insertarEnemigos(dimX, dimY,numEnemies,stage,gameOptions.difLevel);
		}
		//insertEnemies(7);
		level = new Level(dimX, dimY, debug, stage, &gameOptions);
		PointsDestroyMap::resetMap();
		//Enemies::insertarEnemigos(dimX, dimY);
		if(debug){
			insertPlayers(*gameDisplay.userKeyPress, 1, 1);
		}else{
			cout << "Numero de Players: " << gameOptions.numPlayers << endl;
			cout<<"Numero de IAs: "<<gameOptions.numIAPlayers<<endl;
			insertPlayers(*gameDisplay.userKeyPress, gameOptions.numPlayers, gameOptions.numIAPlayers);
		}
	
		unsigned int pixelsX = window.getSize().x;
		unsigned int pixelsY = window.getSize().y;
			
		sf::View view(sf::FloatRect(0.f, 0.f, pixelsX, pixelsY));
		view.move(sf::Vector2f(0, -48));
		window.setView(view);
	}

	void restartGame(sf::RenderWindow& window,GameDisplayController &gameDisplay){
		timesUp=false;
		cout<<"Enemigos antes de reiniciar: "<<Enemies::getVectorEnemies().size()<<endl;
		deleteMap();
		
		cout<<"Enemigos después de reiniciar : "<<Enemies::getVectorEnemies().size()<<endl;
		//startNewGame(window,gameDisplay);
		
	}

	void passLevel(){
		if(stage<=MAX_NUMBER_OF_STAGES){
			stage=stage+1;
		}
		else{
			
			//PANTALLA DE VICTORIA FINAL

		}

		
	}

	void newStage(sf::RenderWindow& window,GameDisplayController &gameDisplay){
		deleteMap();
		stage++;
		Enemies::insertarEnemigos(dimX, dimY,DEFAULT_NUM_OF_ENEMIES*gameOptions.difLevel+(stage/0.25),stage,gameOptions.difLevel);
		startNewGame(window,gameDisplay);
		
	}

	void deleteMap(){
		delete level;
		auto it = Enemies::getVectorEnemies().begin();
		while (it != Enemies::getVectorEnemies().end()) {
				it->reset();
				it = Enemies::getVectorEnemies().erase(it);
		}
		Enemies::getVectorEnemies().clear();
		auto it2 =  PLayers::getVectorPlayer().begin();
		while (it2 !=  PLayers::getVectorPlayer().end()) {
				it2->reset();
				it2 = PLayers::getVectorPlayer().erase(it2);
		}
		PLayers::getVectorPlayer().clear();

		
	}

	void updatePlayers( GameDisplayController& gameDisplay) {
		int ply=1;
		for (Player_ptr player : PLayers::getVectorPlayer()) {
			switch(ply){
				case 1:
					player->updatePlayer(SIZE_PILLAR,SIZE_PILLAR);
				
					break;
				
				case 2:
					player->updatePlayer(SIZE_PILLAR,(dimY-1)*SIZE_PILLAR);
					//level->checkSpawn(SIZE_PILLAR,(dimY-1)*SIZE_PILLAR);
					break;

				case 3:
					player->updatePlayer((dimX-1)*SIZE_PILLAR,SIZE_PILLAR);
					//level->checkSpawn((dimX-1)*SIZE_PILLAR,SIZE_PILLAR);
					break;

				case 4:
					player->updatePlayer((dimX-1)*SIZE_PILLAR,(dimY-1)*SIZE_PILLAR);
					//level->checkSpawn((dimX-1)*SIZE_PILLAR,(dimY-1)*SIZE_PILLAR);
					break;
				
				default:
				break;

			}
			

			//player->updatePlayer();
			player->playerActions();

			level->checkAndFixCollisions(player);
			if (colissionWithEnemies(player)) {
				//player->setExpiredEntity();
			}	
			
			ply++;
		}
	}

	void moveCamera(GameDisplayController& gameDisplay) {
		gameDisplay.updateCamera();

		if (PLayers::getVectorPlayer().size() == 1) {

			sf::Vector2f distCenter2Player =  PLayers::getVectorPlayer()[0]->getCenterPosition() - gameDisplay.camera.getCenter();
			
			sf::Vector2f squareRadius = sf::Vector2f(gameDisplay.getWindow()->getSize().x / 6, gameDisplay.getWindow()->getSize().y / 6);
			if (distCenter2Player.x > squareRadius.x) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + (distCenter2Player.x - squareRadius.x) / 20, gameDisplay.camera.getCenter().y));
			}
			else if (distCenter2Player.x < -squareRadius.x) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + (distCenter2Player.x + squareRadius.x) / 20, gameDisplay.camera.getCenter().y));
			}

			if (distCenter2Player.y > squareRadius.y) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + (distCenter2Player.y - squareRadius.y) / 20));
			}
			else if (distCenter2Player.y < -squareRadius.y) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + (distCenter2Player.y + squareRadius.y) / 20));
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

				if (distCenter2Player.x > squareRadius.x) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + (distCenter2Player.x - squareRadius.x) / 20, gameDisplay.camera.getCenter().y));
				}
				else if (distCenter2Player.x < -squareRadius.x) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + (distCenter2Player.x + squareRadius.x) / 20, gameDisplay.camera.getCenter().y));
				}

				if (distCenter2Player.y > squareRadius.y) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + (distCenter2Player.y - squareRadius.y) / 20));
				}
				else if (distCenter2Player.y < -squareRadius.y) {
					gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + (distCenter2Player.y + squareRadius.y) / 20));
				}

				if (distCenter2Player.y > zoomMoreRadius.y || distCenter2Player.y < -zoomMoreRadius.y) {
					gameDisplay.camera.zoom(1.005);
				}
				if (distCenter2Player.x > zoomMoreRadius.x || distCenter2Player.x < -zoomMoreRadius.x) {
					gameDisplay.camera.zoom(1.005);
				}

				if (zoomValue < 1.2 && (distCenter2Player.y < zoomLessRadius.y && distCenter2Player.y > -zoomLessRadius.y) && (distCenter2Player.x < zoomLessRadius.x && distCenter2Player.x > -zoomLessRadius.x)) {
					gameDisplay.camera.zoom(0.999);
				}
				
			}
		}

		gameDisplay.updateCamera();
	}

	void update(GameDisplayController &gameDisplay) {
		PointsDestroyMap::updateMap();
		level->update();

		if (gameOptions.historyMode) {
			if (level->levelFinished) {
				if (stage < 3) {
					gameDisplay.setGameState(GameDisplayController::VICTORY);
					gameDisplay.notifyChangeDisplay();
				}
				else {
					gameDisplay.setGameState(GameDisplayController::FINAL_SCORE);
					gameDisplay.notifyChangeDisplay();
				}
			}
		}
		else {
			if (gameOptions.numTeams != 0) {
				bool team1Alive = false;
				bool team2Alive = false;

				for (auto player : PLayers::getVectorPlayer()) {
					if (player->lives > 1) {
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
					if (player->lives > 1) {
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

		int totalLives=0;
		for (Player_ptr player : PLayers::getVectorPlayer()) {
			totalLives+=player->getLives();
		}
		
		if(totalLives==0){	
			gameDisplay.setGameState(GameDisplayController::GameState::GAME_OVER);
		}

		//Update camera:
		moveCamera(gameDisplay);
		
		GlobalTime::timeLeft = GlobalTime::totalTime -GameTime::getTimeNow();;
		if (GlobalTime::timeLeft <= 0 && !timesUp) {
			timesUp=true;
			//gameDisplay.setGameState(GameDisplayController::GameState::VICTORY);
			Enemies::insertarEnemigosExtra(dimX,dimY,5);
		}
	}

	void drawPlayers(sf::RenderWindow& w) {

		for (Player_ptr player : PLayers::getVectorPlayer()) {
			if (!player->dead && !player->respawning) {
				w.draw(*player);
				w.draw(player->playerUpdateColor());
			}

			
			if (player->getBomb() != nullptr) {
				auto bombpos = player->getBomb()->getPosition();
				auto playerPos = player->getCenterPosition();

				player->getBomb()->setPosition(playerPos.x, playerPos.y - player->getTextureRect().height + 10);
				w.draw(*player->getBomb());
				player->getBomb()->setPosition(bombpos);
			}

#ifdef HITBOX_DEBUG_MODE
			PlayerIA_ptr pIA;
			if((pIA = std::dynamic_pointer_cast<PlayerIAEntity>(player)) != nullptr){
				pIA->drawMovements(w);
			}
			player->drawEntityHitbox(w);
#endif
		}
	}

	void drawEnemies(sf::RenderWindow& w) {
		for (Enemy_ptr e : Enemies::getVectorEnemies()) {
			w.draw(*e);
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

	bool colissionWithEnemies(Entity_ptr eCol) {
		bool intersec = false;
		for (Enemy_ptr e : Enemies::getVectorEnemies()) {
			intersec = intersec || (e->CanHurtPlayer() && e->collision(*eCol));
			if(intersec){
				e->onCollission(e, eCol, CollisionType::NONE);
			} 
		}
		return intersec;
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
				if(numEnemigos==0){
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
