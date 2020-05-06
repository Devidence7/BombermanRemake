#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <SFML/Graphics.hpp>
#include <memory>

#include "../Include/EntitiesInclude.hpp"
#include "../Textures/WallTexture.h"

#include "../Include/global.hpp"
#include "../Logic/Random.h"

/**
 *
 * Esta clase se encarga de gestionar la l�gica del juego
 *
 */
class Level
{
	// Entities to update vector:
	static std::vector<Entity_ptr> entities;
	static std::vector<Bomb_ptr > onFlightBombs;
	static std::vector<std::vector<Entity_ptr>> miniMap;
	static sf::RectangleShape flooro;

public:
	Level(int dimX, int dimY);
	
	static void update();

	/*
	 * This is a DEBUG method, draws in the RenderWindow the hitbox of the Entity
	*/

	static void draw(sf::RenderWindow &w);
	static bool createFire(int type, int posX, int posY);
	static void createFires(Bomb &b);
	static void checkAndFixCollisions(Entity_ptr eCollisioning);

	////////////////////////////////////////////////
	/////////	MAP OPERATORS	////////////////////
	////////////////////////////////////////////////

	static sf::Vector2f getMapCellCorner(sf::Vector2f pos);
	static Entity_ptr &getCellObject(int x, int y);
	static Entity_ptr &getCellMiniMapObject(int x, int y);
	static Entity_ptr &getCellMiniMapObject(sf::Vector2i pos);
	static Entity_ptr &getCellObject(sf::Vector2i pos);
	static void addEntityToMap(Entity_ptr e, int x, int y);
	static void addEntityToMap(Entity_ptr e, sf::Vector2i pos);
	static void addEntityToMiniMap(Entity_ptr e, int x, int y);
	static void addEntityToMiniMap(Entity_ptr e, sf::Vector2i pos);
	static void addNewItem(Entity_ptr e);
	static void addEntity(Entity_ptr e);
	static void addPillar(int x, int y);
	static void addWall(int x, int y);
	static bool addBomb(Player_ptr p);

	static bool areBombNear(Player_ptr p, sf::Vector2i &bombPosMap);
	static bool canTakeBomb(Player_ptr p);
	static bool canKickBomb(Player_ptr p);
	static void ThrowBomb(Player_ptr p, Bomb_ptr b);
	static void reiniciar(int dimX, int dimY);
};