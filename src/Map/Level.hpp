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
	std::vector<Entity_ptr> entities;

	std::vector<std::vector<Entity_ptr>> miniMap;
	sf::RectangleShape floor;

public:
	Level(std::vector<Enemy_ptr> &enemies, int dimX, int dimY)
	{
		// Reserve space for faster insert, delete of the entities
		entities.reserve(10000);
		
		// Create map matrix:
		EntityMap::entityMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
		miniMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
		// Background:
		floor.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));
		floor.setFillColor(sf::Color(0, 100, 0));

		// Create all pillars:
		for (int x = 0; x < dimX + 2; x++)
		{
			addPillar(x, 0);
			addPillar(x, dimY + 1);
		}

		for (int y = 0; y < dimY + 2; y++)
		{
			addPillar(0, y);
			addPillar(dimX + 1, y);
		}

		//create Pillars
		for (int x = 1; x < dimX + 1; x++)
		{
			for (int y = 1; y < dimY + 1; y++)
			{
				if (x % 2 == 1 || y % 2 == 1)
				{
					// Create random Bricks:
					if ((y > 3|| x > 3 || (y != 3 && x == 2) || (x == 3 && y <= 3)) && !Random::getIntNumberBetween(0, 3))
					{
						bool intersec = false;
						for (Enemy_ptr e : enemies)
						{
							sf::Vector2i p = getMapCoordinates(e->getCenterPosition());
							if (p.x == x && p.y == y)
							{
								intersec = true;
								break;
							}
						}
						if (!intersec)
						{
							addWall(x, y);
						}
					}
				}
				else
				{
					addPillar(x, y);
				}
			}
		}
	}

	
	void update()
	{
		auto it = entities.begin();
		int counter = 0;
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != entities.end())
		{
			// Update the entities.
			(*it)->update();
			if ((*it)->getExpiredEntity())
			{
				// If it is a bomb
				std::shared_ptr<Bomb> b;
				if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr)
				{
					// Create fires
					createFires(*b);
					// When adding entities vector iterator can be invalidated:
					it = entities.begin() + counter;
				}
				else if (std::dynamic_pointer_cast<BrickWall>((*it)) != nullptr)
				{
					if (!Random::getIntNumberBetween(0, 3))
					{
						int randomObject = Random::getIntNumberBetween(0, 3);
						Entity_ptr powerUp;
						switch (randomObject)
						{
						case 0:
							powerUp = std::make_shared<MoreFirePowerUp>(MoreFirePowerUp((*it)->getPosition()));
							break;
						case 1:
							powerUp = std::make_shared<LessFirePowerUp>(LessFirePowerUp((*it)->getPosition()));
							break;
						case 2:
							powerUp = std::make_shared<MoreBombsPowerUp>(MoreBombsPowerUp((*it)->getPosition()));
							break;
						default:
							powerUp = std::make_shared<MoreSpeedPowerUp>(MoreSpeedPowerUp((*it)->getPosition()));
							break;
						}

						addEntityToMiniMap(powerUp, getMapCoordinates((*it)->getPosition()));
						//addNewItem(powerUp);
					}
					else
					{
						// Do this always?
						this->getCellMiniMapObject(getMapCoordinates((*it)->getPosition())).reset();
						this->getCellObject(getMapCoordinates((*it)->getPosition())).reset();
					}
				}
				else
				{
					// Do this always?
					this->getCellMiniMapObject(getMapCoordinates((*it)->getPosition())).reset();
					this->getCellObject(getMapCoordinates((*it)->getPosition())).reset();
				}

				// Remove the entity from the list of entities if it expired.

				it->reset();
				it = entities.erase(it);
			}
			else
			{
				++it;
				counter++;
			}
		}
	}

	/*
	 * This is a DEBUG method, draws in the RenderWindow the hitbox of the Entity
	*/

	void draw(sf::RenderWindow &w)
	{
		// Draw the floor:
		w.draw(floor);

		// Draw the entities
		for (std::vector<Entity_ptr> &v : miniMap)
		{
			for (Entity_ptr &e : v)
			{
				if (e != nullptr)
				{
					w.draw(*e);
					#ifdef HITBOX_DEBUG_MODE
						e->drawEntityHitbox(w);
					#endif
				}
			}
		}
	}

	bool createFire(int type, int posX, int posY)
	{
		// Get the object in cell
		Entity_ptr e = getCellMiniMapObject(getMapCoordinates(posX, posY));

		if (e && e->getIsFireDestroyable() && !e->getFireCanGoThroght())
		{
			e->setExpiredEntity();
			addEntity(e);
		}

		if (!e || e->getFireCanGoThroght())
		{
			e = nullptr;
			Entity_ptr f = std::make_shared<Fire>(Fire(type));
			f->setPosition(posX, posY);
			addNewItem(f);
			//addEntity(f);
			//addEntityToMiniMap(f, posX, posY);
			return false;
		}
		return true;
	}

	void createFires(Bomb &b)
	{
		sf::Vector2f pos = b.getPosition();

		// Central:
		createFire(0, pos.x, pos.y);
		// Down:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 2 : 1;
			if (createFire(fireType, pos.x, pos.y + 48 * i))
			{
				break;
			}
		}
		// Up:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 3 : 1;
			if (createFire(fireType, pos.x, pos.y - 48 * i))
			{
				break;
			}
		}
		// Left:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 6 : 4;
			if (createFire(fireType, pos.x - 48 * i, pos.y))
			{
				break;
			}
		}
		// Right:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 5 : 4;
			if (createFire(fireType, pos.x + 48 * i, pos.y))
			{
				break;
			}
		}
	}


	void checkAndFixCollisions(Entity_ptr eCollisioning)
	{
		sf::Vector2f centerPosition = eCollisioning->getCenterPosition();
		sf::Vector2i position = getMapCoordinates(centerPosition);
		Entity_ptr currentPos;
		if ((currentPos = this->getCellMiniMapObject(position)) != nullptr)
		{
			//this->getOrDie(*eCollisioning, currentPos);
			currentPos->onCollission(eCollisioning, CollisionType::NONE);
		}
		//detectar caso
		sf::FloatRect body = eCollisioning->getGlobalBounds();
		float size = body.width / 2;
		sf::Vector2i position_plus = getMapCoordinates(centerPosition.x + size, centerPosition.y + size);
		sf::Vector2i position_minus = getMapCoordinates(centerPosition.x - size, centerPosition.y - size);
		bool horizontal = false;
		bool vertical = false;
		sf::Vector2i position2(position.x, position.y);
		//ver posibilidad colision horizontal
		if (position_plus.x != position.x)
		{
			//sobre sale por la dch
			position2.x++;
			horizontal = true;
		}
		else if (position_minus.x != position.x)
		{
			//sobre sale por la izq
			position2.x--;
			horizontal = true;
		}

		//ver posiblida colision vertical
		if (position_plus.y != position.y)
		{
			//sobre sale por la dch
			position2.y++;
			vertical = true;
		}
		else if (position_minus.y != position.y)
		{
			//sobre sale por la izq
			position2.y--;
			vertical = true;
		}

		if (!horizontal && !vertical)
		{
			//Dentro de la celda. Sin colision (Caso 0)
			//No hacer nada
		}
		else if ((!horizontal && vertical) || (horizontal && !vertical))
		{
			/******CASOS*************
			 * 	  | x | p |  
			 * 	     OR    
			 * 	    | p |
			 * 	    | X |
			************************/
			Entity_ptr e;
			if ((e = this->getCellMiniMapObject(position2.x, position2.y)) != nullptr)
			{
				sf::Vector2f pos = eCollisioning->getPosition();
				if (horizontal)
				{
//					pos.x += moveGetOrDie_x(*eCollisioning, e);
					e->onCollission(eCollisioning, CollisionType::HORIZONTAL);
				}
				else if (vertical)
				{
					e->onCollission(eCollisioning, CollisionType::VERTICAL);
				}
			}
		}
		else
		{
			sf::Vector2f RealPos = eCollisioning->getPosition();
			//Posible interseccion en 3 celdas
			Entity_ptr e_horizontal, e_vertical, e_diagonal;
			int numIntersecciones = 0;
			e_horizontal = this->getCellMiniMapObject(position2.x, position.y);
			numIntersecciones++;
			e_vertical = this->getCellMiniMapObject(position.x, position2.y);
			numIntersecciones++;
			e_diagonal = this->getCellMiniMapObject(position2.x, position2.y);
			numIntersecciones++;

			if (e_horizontal && e_vertical)
			{
				/******CASOS*************
				 * | x | p |
				 * | ? | x |
				 * 
				************************/
				e_horizontal->onCollission(eCollisioning, CollisionType::HORIZONTAL);
				e_vertical->onCollission(eCollisioning, CollisionType::VERTICAL);
			}
			else if (e_diagonal && e_horizontal)
			{
				/******CASOS*************
				 * | x | p |
				 * | x |   |
				************************/
				e_horizontal->onCollission(eCollisioning, CollisionType::HORIZONTAL);
			}
			else if (e_diagonal && e_vertical)
			{
				/******CASOS*************
				 * |   | p |
				 * | x | x |
				************************/
				e_vertical->onCollission(eCollisioning, CollisionType::VERTICAL);
			}
			else
			{

				Entity_ptr col;
				if (e_horizontal)
				{
					/******CASOS*************
				 * | x | p |
				 * |   |   |
				************************/
					col = e_horizontal;
				}
				if (e_vertical)
				{
					/******CASOS*************
				 * |   | p |
				 * |   | x |
				************************/
					col = e_vertical;
				}
				if (e_diagonal)
				{
					/******CASOS*************
				 * |   | p |
				 * | x |   |
				************************/
					col = e_diagonal;
				}
				if (!col)
				{ //Si no hay colision -> fin
					//return;
				}
				else
				{
					col->onCollission(eCollisioning, CollisionType::CORNER);
					RealPos = eCollisioning->getPosition();
				}
			}
		}
	}

	////////////////////////////////////////////////
	/////////	MAP OPERATORS	////////////////////
	////////////////////////////////////////////////

	sf::Vector2f getMapCellCorner(sf::Vector2f pos)
	{
		return sf::Vector2f((int)pos.x / SIZE_PILLAR * SIZE_PILLAR, (int)pos.y / SIZE_PILLAR * SIZE_PILLAR);
	}

	Entity_ptr &getCellObject(int x, int y)
	{
		if (EntityMap::entityMap[y][x].get() != nullptr && EntityMap::entityMap[y][x].get()->getExpiredEntity())
		{
			EntityMap::entityMap[y][x].reset();
		}
		return EntityMap::entityMap[y][x];
	}

	Entity_ptr &getCellMiniMapObject(int x, int y)
	{
		if (miniMap[y][x].get() != nullptr && miniMap[y][x].get()->getExpiredEntity())
		{
			miniMap[y][x].reset();
		}
		return miniMap[y][x];
	}

	Entity_ptr &getCellMiniMapObject(sf::Vector2i pos)
	{
		return getCellMiniMapObject(pos.x, pos.y);
	}
	Entity_ptr &getCellObject(sf::Vector2i pos)
	{
		return getCellObject(pos.x, pos.y);
	}

	void addEntityToMap(Entity_ptr e, int x, int y)
	{
		getCellObject(x, y) = e;
	}

	void addEntityToMap(Entity_ptr e, sf::Vector2i pos)
	{
		addEntityToMap(e, pos.x, pos.y);
	}
	void addEntityToMiniMap(Entity_ptr e, int x, int y)
	{
		getCellMiniMapObject(x, y) = e;
	}

	void addEntityToMiniMap(Entity_ptr &e, sf::Vector2i pos)
	{
		addEntityToMiniMap(e, pos.x, pos.y);
	}
	void addNewItem(Entity_ptr &e)
	{
		addEntity(e);
		addEntityToMiniMap(e, getMapCoordinates(e->getCenterPosition()));
	}

	void addEntity(Entity_ptr e)
	{
		entities.push_back(e);
	}

	

	void addPillar(int x, int y)
	{
		Entity_ptr e = std::make_shared<Pillar>(Pillar(x, y));
		addEntityToMap(e, x, y);
		addEntityToMiniMap(e, x, y);
	}

	void addWall(int x, int y)
	{
		Entity_ptr e = std::make_shared<BrickWall>(BrickWall(x, y));
		addEntityToMap(e, x, y);
		addEntityToMiniMap(e, x, y);
	}
};