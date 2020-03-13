#pragma once
#include <SFML/Graphics.hpp>
#include "Include/global.hpp"
#include "Entities/Bomb.h"
#include "Map/Map.hpp"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Textures/TextureStorage.h"

using namespace sf;



/**
 *
 * Esta clase se encarga de gestionar la lógica del juego
 *
 */

class Level {
	int level = 1;

	std::vector<Entity_ptr> entities;

public:
	Map map;

	Level(int _level, WallTexture& bw) : level(_level), map(bw) {
		entities.reserve(10000);
	}
	void checkAndFixCollisions(Entity& e) {
		map.checkAndFixCollisions(e, entities);
	}

	void update() {
		auto it = entities.begin();
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != entities.end()) {
			
			// Update the entities.
			(*it)->update();
			if ((*it)->expiredEntity) {

				// If it is a bomb
				std::shared_ptr<Bomb> b;
				if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr) {
					int pos = entities.begin() - it;

					// Create fires
					createFires(b.get());

					// When adding entities vector iterator can be invalidated:
					it = entities.begin() + pos;
				}

				// Remove the entity from the list of entities if it expired.
				it->reset();
				it = entities.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void draw(RenderWindow& w) {
		// Draw the level map
		map.Draw(w);

		// Draw the entities
		for (auto e : entities) {
			w.draw(*e);
		}
	}

	void addEntity(Entity_ptr e) {
		entities.push_back(e);
	}

	void createFire(Bomb* b, int type, int posX, int posY) {
		// If it is a bomb
		Entity_ptr e = map.getCellObject(map.getMapCoordinates(posX, posY));
		std::shared_ptr<BrickWall> bw;
		std::shared_ptr<Pillar> p;
		if (e != nullptr && ((bw = std::dynamic_pointer_cast<BrickWall>(e)) != nullptr)) {
			bw->isDestroyed = true;
		}
		else if (e != nullptr && ((p = std::dynamic_pointer_cast<Pillar>(e)) != nullptr)) {
			// Do nothing
		}
		else {
			//Collider2d colFire(sf::Vector2f(0, 0), sf::FloatRect(0, 0, 48, 48), true);
			std::shared_ptr<Fire> f = std::make_shared<Fire>(Fire(b->getFireTexture(), type));
			f->setPosition(posX, posY);
			addEntity(f);
		}
	}

	void createFires(Bomb* b) {
		// Central:
		createFire(b, 0, b->getPosition().x, b->getPosition().y);
		// Down:
		createFire(b, 2, b->getPosition().x, b->getPosition().y + 48);
		// Up:
		createFire(b, 3, b->getPosition().x, b->getPosition().y - 48);
		// Left:
		createFire(b, 6, b->getPosition().x - 48, b->getPosition().y);
		// Right:
		createFire(b, 5, b->getPosition().x + 48, b->getPosition().y);
	}
};


/**
 *
 * Clase encargada de inicialización de niveles.
 * Mostrará al comienzo el menú
 * Mostrará al morir el Game Over
 */

class Game {
private:
	WallTexture ball_walls;
	TextureStorage textureStorage;
	Level level;
	Sprite BackgroundSprite;
	PlayerEntity *player;

public:
	Game() : level(1, ball_walls){
		player = new PlayerEntity(textureStorage.getPlayerTexture());
		std::shared_ptr<EnemyEntity> e1 = std::make_shared<EnemyEntity>(Balloon(textureStorage.getEnemyTexture()));
		std::shared_ptr<EnemyEntity> e2 = std::make_shared<EnemyEntity>(Ice(textureStorage.getEnemyTexture()));
		std::shared_ptr<EnemyEntity> e3 = std::make_shared<EnemyEntity>(Barrel(textureStorage.getEnemyTexture()));
		std::shared_ptr<EnemyEntity> e4 = std::make_shared<EnemyEntity>(Coin(textureStorage.getEnemyTexture()));
		std::shared_ptr<EnemyEntity> e5 = std::make_shared<EnemyEntity>(Blob(textureStorage.getEnemyTexture()));
		std::shared_ptr<EnemyEntity> e6 = std::make_shared<EnemyEntity>(Ghost(textureStorage.getEnemyTexture()));
		std::shared_ptr<EnemyEntity> e7 = std::make_shared<EnemyEntity>(Hypo(textureStorage.getEnemyTexture()));
		level.addEntity(e1);
		level.addEntity(e2);
		level.addEntity(e3);
		level.addEntity(e4);
		level.addEntity(e5);
		level.addEntity(e6);
		level.addEntity(e7);
	}
	void start();

	void update() {
		level.update();

		if (player->updatePlayer()) {
			// If there is nothing in that cell:
			//if(level.map.getCellObject(level.map.getMapCoordinates(player.getCenterPosition())) == nullptr) {
			//Collider2d colFire(sf::Vector2f(0, 0), sf::FloatRect(0, 0, 1, 1), true);
			std::shared_ptr<Bomb> b = std::make_shared<Bomb>(Bomb(textureStorage.getBombTexture(), textureStorage.getFireTexture()));
			b->setPosition(level.map.getMapCellCorner(player->getCenterPosition()));
			level.addEntity(b);
			//}
		}
		level.checkAndFixCollisions(*player);

	}

	void draw(RenderWindow& w) {
		level.draw(w);
		w.draw(*player);
	}
};
