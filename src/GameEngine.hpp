#pragma once
#include <SFML/Graphics.hpp>
#include <list>

#include "Exceptions/ExceptionsGame.hpp"

#include "Map/Map.hpp"
#include "Entities/Player.h"
#include "Textures/ball_wall_Texture.h"


using namespace sf;


/**
 *
 * Esta clase se encarga de gestionar la lógica del juego
 *
 */

class Level {
	int level = 1;
	Map map;
	std::vector<Entity> entities;

public:
	Level(int _level, Ball_Wall& bw) : level(_level), map(bw) {
	}

	void update() {
		auto it = entities.begin();

		while (it != entities.end()) {
			it->update();
			if (it->expiredEntity) {
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
		for (Entity& e : entities) {
			w.draw(e);
		}
	}


	void addEntity(Entity& e) {
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
	Ball_Wall ball_walls;
	Level level;
	Sprite BackgroundSprite;
	PlayerEntity player;
public:
	Game() : level(1, ball_walls) {

	}
	void start();

	void update() {
		level.update();
		player.update();
	}

	void draw(RenderWindow& w) {
		level.draw(w);
		w.draw(player);
	}
};
