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

	Game() {
	//	cout<<gameOptions.numPlayers<<endl;
		PLayers::insertPlayers(gameOptions.numPlayers);
		
	//	PLayers::insertPlayers();
		Enemies::insertarEnemigos(dimX, dimY);
		level = new Level(dimX, dimY);
		//mainMenu(w);
	}
	void start(sf::RenderWindow& w) {
		//	mainMenu.draw(w);
	}

	void startNewGame(sf::RenderWindow& window){
	//	PLayers::insertPlayers(gameOptions.numPlayers);
		unsigned int pixelsX = window.getSize().x;
		unsigned int pixelsY = window.getSize().y;
		
	//	PLayers::insertPlayers(gameOptions.numPlayers);
		sf::View view(sf::FloatRect(0.f, 0.f, pixelsX, pixelsY));
		view.move(sf::Vector2f(0, -48));
		window.setView(view);

		GameTime::startGameTime();
		GameMusic::playWorld1Music();
	}

	void restartGame(sf::RenderWindow& window){
		//GameTime::startGameTime();
			for (Player_ptr& player : PLayers::getVectorPlayer()) {
		
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
		for (Player_ptr& player : PLayers::getVectorPlayer()) {
				if (player->updatePlayer(ply)) {
					// If there is nothing in that cell:
					Entity_ptr b = std::make_shared<Bomb>(Bomb(player));
					b->setPosition(level->getMapCellCorner(player->getCenterPosition()));
					level->addNewItem(b);
				}




			//player->updatePlayer();
			player->playerActions(ply);




			level->checkAndFixCollisions(player);
			if (colissionWithEnemies(player)) {
				player->setExpiredEntity();
			}	
			ply++;
		
		}
	}

	void update(GameDisplayController &gameDisplay) {
		level->update();
		updatePlayers(gameDisplay);
		updateEnemies();
		int totalLives=0;
		for (Player_ptr& player : PLayers::getVectorPlayer()) {
			totalLives+=player->getLives();
		}
		//cout<<totalLives<<endl;
		if(totalLives==0){
			
			gameDisplay.setGameState(GameDisplayController::GameState::GAME_OVER);
		}
	}

	void drawPlayers(sf::RenderWindow& w) {

		for (Player_ptr& player : PLayers::getVectorPlayer()) {
			w.draw(*player);
			w.draw(player->playerUpdateColor());
#ifdef HITBOX_DEBUG_MODE
			player->drawEntityHitbox(w);
#endif
		}
	}

	void drawEnemies(sf::RenderWindow& w) {
		for (Enemy_ptr& e : Enemies::getVectorEnemies()) {
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
		for (Enemy_ptr& e : Enemies::getVectorEnemies()) {
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
