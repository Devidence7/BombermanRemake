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
/*enum dificultad{
	EASY,
	NORMAL,
	HARD
};*/




public:
	struct GameOptions {
	
		int numPlayers;
	//	double difLevel;
		
	};
	GameOptions gameOptions;

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
	Enemies::insertarEnemigos(dimX, dimY);
}


	void insertPlayers(UserKeyPress &userKeyPress, int numPlayers) {
		for (int i = 0; i < numPlayers; i++) {
			PLayers::addPlayer(userKeyPress.getPlayerControls(i+1));
		}
	}

	void insertEnemies(int numEnemigos){
		Enemies::insertarEnemigos(dimX, dimY);
	}

	void startNewGame(sf::RenderWindow& window, GameDisplayController &gameDisplay){
		//	cout<<gameOptions.numPlayers<<endl;
		//Enemies::insertarEnemigos(dimX, dimY);
		//insertEnemies(7);
		level = new Level(dimX, dimY);
		//Enemies::insertarEnemigos(dimX, dimY);
		insertPlayers(*gameDisplay.userKeyPress, gameOptions.numPlayers);
	
		unsigned int pixelsX = window.getSize().x;
		unsigned int pixelsY = window.getSize().y;
			
		sf::View view(sf::FloatRect(0.f, 0.f, pixelsX, pixelsY));
		view.move(sf::Vector2f(0, -48));
		window.setView(view);

		GameTime::startGameTime();
		GameMusic::playWorld1Music();
	}

	void restartGame(sf::RenderWindow& window,GameDisplayController &gameDisplay){
		deleteMap();
		Enemies::insertarEnemigos(dimX, dimY);
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
			cout<<"Update"<<endl;
			(*it)->update();
			cout<<"CheckAndFix"<<endl;
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
