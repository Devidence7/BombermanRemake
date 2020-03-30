#pragma once
#include <SFML/Graphics.hpp>
#include "Include/global.hpp"
#include "Entities/Bomb.h"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Map/Level.hpp"
#include "Textures/TextureStorage.h"

using namespace sf;

/**
 *
 * Clase encargada de inicialización de niveles.
 * Mostrará al comienzo el menú
 * Mostrará al morir el Game Over
 */

class Game {
private:
	// Initialize textures
	TextureStorage textureStorage;
	Level *level;
	PlayerEntity *player;
	//std::vector<Enemy_ptr> enemies;

public:
	Game() {
		level = new Level();
		player = new PlayerEntity();
		// Enemy_ptr e1 = std::make_shared<EnemyEntity>(Balloon());
		// Enemy_ptr e2 = std::make_shared<EnemyEntity>(Ice());
		// Enemy_ptr e3 = std::make_shared<EnemyEntity>(Barrel());
		// Enemy_ptr e4 = std::make_shared<EnemyEntity>(Coin());
		// Enemy_ptr e5 = std::make_shared<EnemyEntity>(Blob());
		// Enemy_ptr e6 = std::make_shared<EnemyEntity>(Ghost());
		// Enemy_ptr e7 = std::make_shared<EnemyEntity>(Hypo());
		// enemies.push_back(e1);
		// enemies.push_back(e2);
		// enemies.push_back(e3);
		// enemies.push_back(e4);
		// enemies.push_back(e5);
		// enemies.push_back(e6);
		// enemies.push_back(e7);
	}
	void start();

	void update() {
		level->update();

		if (player->updatePlayer()) {
			// If there is nothing in that cell:
			//if(level.map.getCellObject(level.map.getMapCoordinates(player.getCenterPosition())) == nullptr) {
			//Collider2d colFire(sf::Vector2f(0, 0), sf::FloatRect(0, 0, 1, 1), true);
			Entity_ptr b = std::make_shared<Bomb>(Bomb(player));
			b->setPosition(level->getMapCellCorner(player->getCenterPosition()));
			level->addNewItem(b);
			//level->addEntity(b);
			//level->addEntityToMiniMap(b, level->getMapCoordinates(b->getPosition() ) );
			//}
		}
		//for(Enemy_ptr &e : this->enemies){
		//	e->update();
		//	level->checkAndFixCollisions2(*e);
		//}
		level->checkAndFixCollisions2(*player);

	}

	void draw(RenderWindow& w) {
		level->draw(w);
		//for(Enemy_ptr &e : this->enemies){
		//	w.draw(*e);
		//}
		w.draw(*player);
		if (HITBOX_DEBUG_MODE) {
			level->drawEntityHitbox(w, *player);
			//for(Enemy_ptr &e : this->enemies){
			//level->drawEntityHitbox(w, *e);
			//}
		}
	}
};
