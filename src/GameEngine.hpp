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
				if (player->updatePlayer()) {
					// If there is nothing in that cell:
					Entity_ptr b = std::make_shared<Bomb>(Bomb(player));
					b->setPosition(level->getMapCellCorner(player->getCenterPosition()));
					level->addNewItem(b);
				}

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
			gameDisplay.camera.setCenter(PLayers::getVectorPlayer()[0]->getCenterPosition());
		}

		//gameDisplay.camera.ge

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
			gameDisplay.camera.move(sf::Vector2f(0, -1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) {
			gameDisplay.camera.move(sf::Vector2f(0, 1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
			gameDisplay.camera.move(sf::Vector2f(-1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
			gameDisplay.camera.move(sf::Vector2f(1, 0));
		}*/


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
