#pragma once
#include <SFML/Graphics.hpp>

#include "Include/global.hpp"
#include "Entities/Bomb.h"
#include "Map/Map.hpp"
#include "Entities/Player.h"
#include "Textures/TextureStorage.h"
#include "Phisics/collider2d.hpp"

using namespace sf;


/**
 *
 * Esta clase se encarga de gestionar la lógica del juego
 *
 */

class Level {
	int level = 1;
	Map map;
	std::vector<Entity*> entities;

public:
	Level(int _level, WallTexture& bw) : level(_level), map(bw) {
		entities.reserve(10000);
	}
	void checkAndFixCollisions(Entity &e){
		map.checkAndFixCollisions(e);
	}

	void update() {
		auto it = entities.begin();
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != entities.end()) {
			// Update the entities.
			(*it)->update();
			if ((*it)->expiredEntity) {

				// If it is a bomb
				Bomb* b;
				if ((b = dynamic_cast<Bomb*>(*it)) != nullptr) {
					int pos = entities.begin() - it;

					// Create fires
					createFires(b);

					// When adding entities vector iterator can be invalidated:
					it = entities.begin() + pos;
				}

				// Remove the entity from the list of entities if it expired.
				delete(*it);
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

	void addEntity(Entity* e) {
		entities.push_back(e);
	}

	void createFires(Bomb* b) {
		// Central:
		Collider2d colFire(sf::Vector2f(0,0), sf::FloatRect(0,0,48,48), true);
		Fire* f = new Fire(b->getFireTexture(),colFire);
		f->setPosition(b->getPosition());
		addEntity(f);
		// Down:
		f = new Fire(b->getFireTexture(),colFire, 2);
		f->setPosition(b->getPosition().x, b->getPosition().y + 48);
		addEntity(f);
		// Up:
		f = new Fire(b->getFireTexture(),colFire, 3);
		f->setPosition(b->getPosition().x, b->getPosition().y - 48);
		addEntity(f);
		// Left:
		f = new Fire(b->getFireTexture(), colFire, 6);
		f->setPosition(b->getPosition().x - 48, b->getPosition().y);
		addEntity(f);
		// Right:
		f = new Fire(b->getFireTexture(),colFire, 5);
		f->setPosition(b->getPosition().x + 48, b->getPosition().y);
		addEntity(f);
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
	int uno = 1;
	PlayerEntity player;

public:
	Game() : level(1, ball_walls) {
		textureStorage = TextureStorage();
	}
	void start();

	void update() {
		level.update();
		
		if (player.updatePlayer()) {
			Collider2d colFire(sf::Vector2f(0,0), sf::FloatRect(0,0,1,1), true);
			Bomb* b = new Bomb(textureStorage.getBombTexture(), textureStorage.getFireTexture(), colFire);
			b->setPosition(player.getPosition());
			level.addEntity(b);
		}
		level.checkAndFixCollisions(this->player);
		
	}

	void draw(RenderWindow& w) {
		level.draw(w);
		w.draw(player);
	}
};
