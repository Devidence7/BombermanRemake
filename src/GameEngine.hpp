#pragma once
#include <SFML/Graphics.hpp>

#include "Include/global.hpp"
#include "Include/EntitiesInclude.hpp"

#include "Map/Level.hpp"
#include "Textures/TextureStorage.h"
#include "Interface/GameInterface.h"
#include "Music/GameMusic.h"
#include "Interface/GameDisplayController.h"

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
	
	int timeLeft = 299;
	int timeToShow;
	// Initialize textures
	TextureStorage textureStorage;
	int numEnemigos=7;
	//MainMenu mainMenu;
/*enum dificultad{
	EASY,
	NORMAL,
	HARD
};*/




public:
	struct GameOptions {
	
		int numPlayers;
		double difLevel;
		
		
	};
	int stage;
	GameOptions gameOptions;
	bool debug=false;

/*	double getDificultad(dificultad d){
	switch(d){
		case EASY:
			return 1;
			break;
		case NORMAL:
			return 1.5;
			break;
		case HARD:
			return 1.75;
		default:
		break;
	}
} */
Game(){
	stage=1;
}
	int gameTime(){
		return timeLeft;
	}

	void insertPlayers(UserKeyPress &userKeyPress, int numPlayers) {
		for (int i = 0; i < numPlayers; i++) {
			PLayers::addPlayer(userKeyPress.getPlayerControls(i+1));
		}
	}

	/*void insertEnemies(int numEnemigos){
		Enemies::insertarEnemigos(dimX, dimY);
	}*/

	void startNewGame(sf::RenderWindow& window, GameDisplayController &gameDisplay){
		// Restart time parameters
		GameTime::startGameTime();
		GameMusic::playWorld1Music();

		//	cout<<gameOptions.numPlayers<<endl;
		int numEnemies=numEnemigos*gameOptions.difLevel*(stage/0.75);
	
		if(!debug){
			Enemies::insertarEnemigos(dimX, dimY,numEnemies,stage);
		}
		//insertEnemies(7);
		level = new Level(dimX, dimY,debug);
		//Enemies::insertarEnemigos(dimX, dimY);
		insertPlayers(*gameDisplay.userKeyPress, gameOptions.numPlayers);
	
		unsigned int pixelsX = window.getSize().x;
		unsigned int pixelsY = window.getSize().y;
			
		sf::View view(sf::FloatRect(0.f, 0.f, pixelsX, pixelsY));
		view.move(sf::Vector2f(0, -48));
		window.setView(view);
	}

	void restartGame(sf::RenderWindow& window,GameDisplayController &gameDisplay){
		deleteMap();
		cout<<numEnemigos<<endl;
		Enemies::insertarEnemigos(dimX, dimY,numEnemigos*gameOptions.difLevel*(stage/0.75),stage);
		startNewGame(window,gameDisplay);
		
	}

	void passLevel(){
		
		stage=stage+1;
		
	}

	void newStage(sf::RenderWindow& window,GameDisplayController &gameDisplay){
		deleteMap();
		stage++;
		Enemies::insertarEnemigos(dimX, dimY,numEnemigos*gameOptions.difLevel*(stage/0.75),stage);
		startNewGame(window,gameDisplay);
		
	}

	void deleteMap(){
		delete level;
		auto it = Enemies::getVectorEnemies().begin();
		while (it != Enemies::getVectorEnemies().end()) {
				it->reset();
				it = Enemies::getVectorEnemies().erase(it);
		}
		auto it2 =  PLayers::getVectorPlayer().begin();
		while (it2 !=  PLayers::getVectorPlayer().end()) {
				it2->reset();
				it2 = PLayers::getVectorPlayer().erase(it2);
		}
	}

	void updatePlayers( GameDisplayController& gameDisplay) {
		int ply=1;
		for (Player_ptr player : PLayers::getVectorPlayer()) {
			player->updatePlayer();

			//player->updatePlayer();
			player->playerActions();

			level->checkAndFixCollisions(player);
			if (colissionWithEnemies(player)) {
				player->setExpiredEntity();
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
		level->update();
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
		
		timeToShow = timeLeft -GameTime::getTimeNow();;
		if (timeToShow <= 0) {
			//gameDisplay.setGameState(GameDisplayController::GameState::VICTORY);
			Enemies::insertarEnemigosExtra(dimX,dimY);
		}
	}

	void drawPlayers(sf::RenderWindow& w) {

		for (Player_ptr player : PLayers::getVectorPlayer()) {
			w.draw(*player);
			w.draw(player->playerUpdateColor());
#ifdef HITBOX_DEBUG_MODE
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
	}

	bool colissionWithEnemies(Entity_ptr eCol) {
		bool intersec = false;
		for (Enemy_ptr e : Enemies::getVectorEnemies()) {
			intersec = intersec || (e->CanHurtPlayer() && e->collision(*eCol));
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
			}
			
			else {
				++it;
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
