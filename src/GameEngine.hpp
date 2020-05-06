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
	// Initialize textures
	TextureStorage textureStorage;
	//MainMenu mainMenu;
public:
	struct GameOptions {
		//bool multiplayerGame = false;
		int numPlayers=2;
	};
	GameOptions gameOptions;

	void insertPlayers(UserKeyPress &userKeyPress, int numPlayers) {
		for (int i = 0; i < numPlayers; i++) {
			PLayers::addPlayer(userKeyPress.getPlayerControls(i+1));
		}
	}

	void startNewGame(sf::RenderWindow& window, GameDisplayController &gameDisplay){
		//	cout<<gameOptions.numPlayers<<endl;
		level = new Level(dimX, dimY);
		insertPlayers(*gameDisplay.userKeyPress, gameOptions.numPlayers);

		//	PLayers::insertPlayers();
		//Enemies::insertarEnemigos(dimX, dimY);
		
		//mainMenu(w);

	//	PLayers::insertPlayers(gameOptions.numPlayers);
		unsigned int pixelsX = window.getSize().x;
		unsigned int pixelsY = window.getSize().y;
		
	//	PLayers::insertPlayers(gameOptions.numPlayers);

		GameTime::startGameTime();
		GameMusic::playWorld1Music();
	}

	void restartGame(sf::RenderWindow& window){
		//GameTime::startGameTime();
			for (Player_ptr player : PLayers::getVectorPlayer()) {
		
				//player.reset();
				//cout<<"holi"<<endl;
				player->lives=3;
		
			}
			
			//	Enemies::insertarEnemigos(dimX, dimY);
			//GameTime::startGameTime();
		    GameMusic::playWorld1Music();

			//Enemies::insertarEnemigos(dimX, dimY);
			level->reiniciar(dimX,dimY);
		
		//level = new Level(dimX, dimY);
		//startNewGame(window);
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
		if (PLayers::getVectorPlayer().size() == 1) {
			gameDisplay.getWindow()->getSize();
			gameDisplay.getWindow()->getDefaultView().getCenter();
			PLayers::getVectorPlayer()[0]->getCenterPosition();

			sf::Vector2f distCenter2Player =  PLayers::getVectorPlayer()[0]->getCenterPosition() - gameDisplay.camera.getCenter();
			
			float squareRadius = 150;
			if (distCenter2Player.x > squareRadius) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + (distCenter2Player.x - squareRadius) / 20, gameDisplay.camera.getCenter().y));
			}
			else if (distCenter2Player.x < -squareRadius) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x + (distCenter2Player.x + squareRadius) / 20, gameDisplay.camera.getCenter().y));
			}

			if (distCenter2Player.y > squareRadius) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + (distCenter2Player.y - squareRadius) / 20));
			}
			else if (distCenter2Player.y < -squareRadius) {
				gameDisplay.camera.setCenter(sf::Vector2f(gameDisplay.camera.getCenter().x, gameDisplay.camera.getCenter().y + (distCenter2Player.y + squareRadius) / 20));
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
		//cout<<totalLives<<endl;
		if(totalLives==0){
			
			gameDisplay.setGameState(GameDisplayController::GameState::GAME_OVER);
		}

		//Update camera:
		moveCamera(gameDisplay);
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
