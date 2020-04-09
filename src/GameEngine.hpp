#pragma once
#include <SFML/Graphics.hpp>

#include "Include/global.hpp"
#include "Include/EntitiesInclude.hpp"

#include "Map/Level.hpp"
#include "Textures/TextureStorage.h"

using namespace sf;

/**
 *
 * Clase encargada de inicialización de niveles.
 * Mostrará al comienzo el menú
 * Mostrará al morir el Game Over
 */

class Game
{
private:
	// Dim level map:
	int dimY = 15;
	int dimX = 25;
	// Initialize textures
	TextureStorage textureStorage;
	Level *level;
	std::vector<Enemy_ptr> enemies;
	//std::vector<Enemy_ptr> enemies;
public:
	Game()
	{
		PLayers::insertPlayers();
		insertarEnemigos();
		level = new Level(enemies, dimX, dimY);
	}
	void start();

	void updatePlayers(){
		for(Player_ptr &player : PLayers::getVectorPlayer()){
			if (player->updatePlayer())
			{
				// If there is nothing in that cell:
				Entity_ptr b = std::make_shared<Bomb>(Bomb(player));
				b->setPosition(level->getMapCellCorner(player->getCenterPosition()));
				level->addNewItem(b);
			}
			level->checkAndFixCollisions(player);
			if(colissionWithEnemies(player)){
				player->setExpiredEntity();
			}
		}
	}

	void update()
	{
		level->update();
		updatePlayers();
		updateEnemies();
	}

	void drawPlayers(sf::RenderWindow &w){

		for (Player_ptr &player : PLayers::getVectorPlayer())
		{
			w.draw(*player);
#ifdef HITBOX_DEBUG_MODE
			player->drawEntityHitbox(w);
#endif
		}
	}

	void drawEnemies(sf::RenderWindow &w)
	{
		for (Enemy_ptr &e : this->enemies)
		{
			w.draw(*e);
#ifdef HITBOX_DEBUG_MODE
			e->drawEntityHitbox(w);
			e->generateMovements();
			e->drawMovements(w);
#endif
		}
	}

	bool colissionWithEnemies(Entity_ptr eCol)
	{
		bool intersec = false;
		for (Enemy_ptr &e : this->enemies)
		{
			intersec = intersec || (e->CanHurtPlayer() && e->collision(*eCol));
		}
		return intersec;
	}

	void insertarEnemigos()
	{
		Enemy_ptr e1 = std::make_shared<EnemyEntity>(Balloon());
		Enemy_ptr e2 = std::make_shared<EnemyEntity>(Ice());
		Enemy_ptr e3 = std::make_shared<EnemyEntity>(Barrel());
		Enemy_ptr e4 = std::make_shared<EnemyEntity>(Coin());
		Enemy_ptr e5 = std::make_shared<EnemyEntity>(Blob());
		Enemy_ptr e6 = std::make_shared<EnemyEntity>(Ghost());
		Enemy_ptr e7 = std::make_shared<EnemyEntity>(Hypo());
		enemies.push_back(e1);
		enemies.push_back(e2);
		enemies.push_back(e3);
		enemies.push_back(e4);
		enemies.push_back(e5);
		enemies.push_back(e6);
		enemies.push_back(e7);

		for (Enemy_ptr e : enemies)
		{
			int x, y;
			do
			{
				x = Random::getIntNumberBetween(0, dimX / 2);

			} while (x < 3);
			do
			{
				y = Random::getIntNumberBetween(0, dimY / 2);
			} while (y < 3);
			e->setPosition(sf::Vector2f((x * 2 + 1) * SIZE_PILLAR - 3, (y * 2 + 1) * SIZE_PILLAR - 3));
		}
	}

	void updateEnemies()
	{
		auto it = enemies.begin();
		int counter = 0;
		while (it != enemies.end())
		{
			// Update the enemies.
			(*it)->update();
			level->checkAndFixCollisions((*it));
			if ((*it)->getExpiredEntity())
			{
				it->reset();
				it = enemies.erase(it);
			}
			else
			{
				++it;
				counter++;
			}
		}
	}

	void draw(RenderWindow &w)
	{
		level->draw(w);
		drawEnemies(w);
		drawPlayers(w);
	}
};
