#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/Bomb.h"
#include "Map/Map.hpp"
#include "Entities/Player.h"
#include "Textures/TextureStorage.h"


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
	}

	void update() {
		auto it = entities.begin();
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != entities.end()) {
			// Update the entities.
			(*it)->update();
			if ((*it)->expiredEntity) {

				// If it is a bomb
				Bomb *b;
				if ((b = dynamic_cast<Bomb*>(*it)) != nullptr) {
					int pos = entities.begin() - it;
					Fire* f = new Fire(b->getFireTexture());
					f->setPosition(b->getPosition());
					addEntity(f);

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
	PlayerEntity player;
	
public:
	Game() : level(1, ball_walls) {
		textureStorage = TextureStorage();
	}
	void start();

	void update() {
		level.update();
		if (player.updatePlayer())
		{
			Bomb *b = new Bomb(textureStorage.getBombTexture(), textureStorage.getFireTexture());
			b->setPosition(player.getPosition());
			level.addEntity(b);
		}
	}

	void draw(RenderWindow& w) {
		level.draw(w);
		w.draw(player);
	}
};
