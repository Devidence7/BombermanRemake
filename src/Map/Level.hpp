#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <SFML/Graphics.hpp>
#include <memory>

#include "../Entities/Bomb.h"
#include "../Entities/BrickWall.h"
#include "../Entities/Pillar.hpp"
#include "../Entities/PowerUp.h"
#include "../Textures/WallTexture.h"
#include "../Include/global.hpp"
#include "../Logic/Random.h"

typedef std::shared_ptr<Entity> Entity_ptr;
typedef std::shared_ptr<Bomb> Bomb_ptr;
typedef std::shared_ptr<Fire> Fire_ptr;
typedef std::shared_ptr<Pillar> Pillar_ptr;
typedef std::shared_ptr<BrickWall> BrickWall_ptr;
typedef std::shared_ptr<EnemyEntity> Enemy_ptr;



/**
 *
 * Esta clase se encarga de gestionar la l�gica del juego
 *
 */
class Level
{
	// All entities vector:
	std::vector<Entity_ptr> entities;

	// Level map:
	int dimY = 15;
	int dimX = 25;
	std::vector<std::vector<Entity_ptr>> map;
	std::vector<std::vector<Entity_ptr>> miniMap;
	sf::RectangleShape floor;

public:
	Level()
	{
		// Reserve space for faster insert, delete of the entities
		entities.reserve(10000);

		// Create map matrix:
		map = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
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

		for (int x = 1; x < dimX + 1; x++)
		{
			for (int y = 1; y < dimY + 1; y++)
			{
				if (x % 2 == 1 || y % 2 == 1)
				{
					// Create random Bricks:
					if (!Random::getIntNumberBetween(0, 3))
					{
						addWall(x, y);
					}
				}
				else
				{
					addPillar(x, y);
				}
			}
		}
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
				} else if (std::dynamic_pointer_cast<BrickWall>((*it)) != nullptr)
				{
					
					if (!Random::getIntNumberBetween(0, 3))
					{
						int randomObject = Random::getIntNumberBetween(0, 3);
						Entity_ptr powerUp;
						

						switch (randomObject) {
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
					else {
						// Do this always?
						this->getCellMiniMapObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
						this->getCellObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
					}
				}
				else {
					// Do this always?
					this->getCellMiniMapObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
					this->getCellObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
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
	void drawEntityHitbox(sf::RenderWindow &w, Entity &e)
	{
		sf::FloatRect dim = e.getGlobalBounds();
		sf::VertexArray lines(sf::Lines, 2);

		// Left
		lines[0].position = sf::Vector2f(dim.left, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		// top
		lines[0].position = sf::Vector2f(dim.left, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		lines[0].position = sf::Vector2f(dim.left, dim.top + dim.height);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		lines[0].position = sf::Vector2f(dim.left + dim.width, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);
	}

	void draw(sf::RenderWindow &w)
	{
		// Draw the floor:
		w.draw(floor);

		// Draw the entities
		//for (auto e : entities) {
		//	w.draw(*e);
		//	if (HITBOX_DEBUG_MODE) {
		//		drawEntityHitbox(w, *e);
		//	}
		//}
		for (std::vector<Entity_ptr> &v : miniMap)
		{
			for (Entity_ptr &e : v)
			{
				if (e != nullptr)
				{
					w.draw(*e);
					//if(std::dynamic_pointer_cast<PowerUp>(e) != nullptr){
					//	std::cout << "PoweUp\n";
					//}
					if (HITBOX_DEBUG_MODE)
					{
						drawEntityHitbox(w, *e);
					}
				}
			}
		}
	}

	void addEntity(Entity_ptr e)
	{
		entities.push_back(e);
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

	bool circIntersect(Entity &e1, Entity &e2)
	{
		sf::FloatRect shape1 = e1.getGlobalBounds();
		sf::FloatRect shape2 = e2.getGlobalBounds();

		float dx = (shape1.left + shape1.width / 2) - (shape2.left + (shape2.width / 2));
		float dy = (shape1.top + (shape1.width / 2)) - (shape2.top + (shape2.width / 2));
		float distance = std::sqrt((dx * dx) + (dy * dy));

		return (distance <= (shape1.width / 2) + (shape2.width / 2));
	}

	bool intersectsCircleRect(Entity &e1circle, Entity &e2rect, double &x, double &y)
	{
		sf::FloatRect circle = e1circle.getGlobalBounds();
		sf::Vector2f circleC = e1circle.getCenterPosition();
		sf::FloatRect rect = e2rect.getGlobalBounds();
		sf::Vector2f recteC = e2rect.getCenterPosition();

		sf::Vector2f circleDistance;

		//std::cout << "1" << std::endl;

		x = (circleC.x - recteC.x);
		y = (circleC.y - recteC.y);
		circleDistance.x = abs(x);
		circleDistance.y = abs(y);

		//std::cout << "2" << std::endl;

		// circle.width and circle.height should be the same (it is the radius).
		if (circleDistance.x > (rect.width / 2 + circle.width / 2))
		{
			return false;
		}
		if (circleDistance.y > (rect.height / 2 + circle.height / 2))
		{
			return false;
		}

		// Move one pixel modulus
		float moduloCentro = sqrtf(x * x + y * y);
		x /= moduloCentro;
		y /= moduloCentro;

		x /= 1000.0;
		y /= 1000.0;

		if (circleDistance.x <= (rect.width / 2))
		{
			return true;
		}
		if (circleDistance.y <= (rect.height / 2))
		{
			return true;
		}

		double cornerDistance_sq;
		cornerDistance_sq = pow((circleDistance.x - rect.width / 2), 2) +
							pow((circleDistance.y - rect.height / 2), 2);

		return (cornerDistance_sq <= pow(circle.width / 2, 2));
	}

	// TODO: divide between enemies collition and player collitions:
	void checkAndFixCollisions(Entity &eCollisioning)
	{
		//for (Entity_ptr _e : entities) {
		
		//for (std::vector<Entity_ptr> &v : miniMap)
		//{
		//	for (Entity_ptr &_e : v)
		sf::Vector2i position = this->getMapCoordinates(eCollisioning.getCenterPosition());
		for(int i = position.y-1 > 0 ? position.y-1 : 0;i< position.y+2;i++){
		for(int j = position.x-1  > 0 ? position.x-1 : 0; j< position.x+2;j++){
			Entity_ptr _e = getCellMiniMapObject(sf::Vector2i(j,i));
				if(_e == nullptr){
					continue;
				}
				double dumb1, dumb2;
				if (intersectsCircleRect(eCollisioning, *_e, dumb1, dumb2))
				{
					eCollisioning.setCollision();
					PlayerEntity *p;
					std::shared_ptr<PowerUp> pu;
					if (std::dynamic_pointer_cast<Fire>(_e) != nullptr)
					{
						eCollisioning.setExpiredEntity();
					}
					else if (dynamic_cast<PlayerEntity *>(&eCollisioning) != nullptr && std::dynamic_pointer_cast<EnemyEntity>(_e) != nullptr)
					{
						eCollisioning.setExpiredEntity();
					}
					else if ((p = dynamic_cast<PlayerEntity *>(&eCollisioning)) && (pu = std::dynamic_pointer_cast<PowerUp>(_e)))
					{
						pu->setPlayerStatus(*p);
						pu->setExpiredEntity();
						_e = nullptr;
					}
					else
					{
						double x;
						double y;
						while (intersectsCircleRect(eCollisioning, *_e, x, y))
						{
							sf::Vector2f blockCpos = _e->getCenterPosition();
							sf::Vector2i mapPos = getMapCoordinates(blockCpos);

							// This is for not guide Bomberman to a hole if he can't go there
							if (abs(x) > abs(y))
							{
								if (y < 0)
								{
									if (getCellObject(mapPos.x, mapPos.y - 1))
									{
										eCollisioning.move(x, 0);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
								else
								{
									if (getCellObject(mapPos.x, mapPos.y + 1))
									{
										eCollisioning.move(x, 0);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
							}
							else
							{
								if (x < 0)
								{
									if (getCellObject(mapPos.x - 1, mapPos.y))
									{
										eCollisioning.move(0, y);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
								else
								{
									if (getCellObject(mapPos.x + 1, mapPos.y))
									{
										eCollisioning.move(0, y);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
							}
						}
					}
				}
			}
			}
		}

		sf::Vector2i getMapCoordinates(int x, int y)
		{
			return sf::Vector2i((int)x / SIZE_PILLAR, (int)y / SIZE_PILLAR);
		}

		sf::Vector2i getMapCoordinates(sf::Vector2f pos)
		{
			return getMapCoordinates(pos.x, pos.y);
		}

		sf::Vector2f getMapCellCorner(sf::Vector2f pos)
		{
			return sf::Vector2f((int)pos.x / SIZE_PILLAR * SIZE_PILLAR, (int)pos.y / SIZE_PILLAR * SIZE_PILLAR);
		}

		Entity_ptr &getCellObject(int x, int y)
		{
			if (map[y][x].get() != nullptr && map[y][x].get()->getExpiredEntity())
			{
				map[y][x].reset();
			}
			return map[y][x];
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
		void addNewItem(Entity_ptr& e){
			addEntity(e);
			addEntityToMiniMap(e, getMapCoordinates(e->getCenterPosition()));
		}

	};