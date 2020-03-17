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

public:
	Game() {
		level = new Level();
		player = new PlayerEntity();
		std::shared_ptr<EnemyEntity> e1 = std::make_shared<EnemyEntity>(Balloon());
		std::shared_ptr<EnemyEntity> e2 = std::make_shared<EnemyEntity>(Ice());
		std::shared_ptr<EnemyEntity> e3 = std::make_shared<EnemyEntity>(Barrel());
		std::shared_ptr<EnemyEntity> e4 = std::make_shared<EnemyEntity>(Coin());
		std::shared_ptr<EnemyEntity> e5 = std::make_shared<EnemyEntity>(Blob());
		std::shared_ptr<EnemyEntity> e6 = std::make_shared<EnemyEntity>(Ghost());
		std::shared_ptr<EnemyEntity> e7 = std::make_shared<EnemyEntity>(Hypo());
		//level->addEntity(e1);
		//level->addEntity(e2);
		//level->addEntity(e3);
		//level->addEntity(e4);
		//level->addEntity(e5);
		//level->addEntity(e6);
		//level->addEntity(e7);
	}
	void start();

	void update() {
		level->update();

		if (player->updatePlayer()) {
			// If there is nothing in that cell:
			//if(level.map.getCellObject(level.map.getMapCoordinates(player.getCenterPosition())) == nullptr) {
			//Collider2d colFire(sf::Vector2f(0, 0), sf::FloatRect(0, 0, 1, 1), true);
			Entity_ptr b = std::make_shared<Bomb>(Bomb(player->getPowerOfBombs()));
			b->setPosition(level->getMapCellCorner(player->getCenterPosition()));
			level->addNewItem(b);
			//level->addEntity(b);
			//level->addEntityToMiniMap(b, level->getMapCoordinates(b->getPosition() ) );
			//}
		}
		level->checkAndFixCollisions(*player);

	}

	void draw(RenderWindow& w) {
		level->draw(w);
		w.draw(*player);
		if (HITBOX_DEBUG_MODE) {
			level->drawEntityHitbox(w, *player);
		}
	}
};
