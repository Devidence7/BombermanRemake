#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <SFML/Graphics.hpp>

#include "../Entities/BrickWall.h"
#include "../Textures/WallTexture.h"
#include "wall.hpp"
#include "../Entities/mapSprites.hpp"

class Map {
	int dimY = 15;
	int dimX = 25;

	WallTexture& Pillars_Balls;
	std::vector<std::vector<Entity*>> map;

	sf::RectangleShape floor;


	Pillar* createPilar(int x, int y) {
		Pillar* p = new Pillar((sizeColliderPillar)*y, x * (sizeColliderPillar));
		p->setTexture(Pillars_Balls.getTexture());
		p->setTextureRect(Pillars_Balls.getRectPillar());
		p->setPosition(sf::Vector2f((sizeColliderPillar)*y, x * (sizeColliderPillar)));
		return p;
	}

public:
	std::vector<Entity*> wallEntities;


	Map(WallTexture& bw) : Pillars_Balls(bw) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, 3);

		// Create map matrix:
		map = std::vector<std::vector<Entity*>>(dimY + 2, std::vector<Entity*>(dimX + 2));

		// Background:
		floor.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));
		floor.setFillColor(sf::Color(0, 100, 0));

		// Create all pillars:
		for (int x = 0; x < dimY + 2; x++) {
			map[x][0] = createPilar(x, 0);
			map[x][dimX + 1] = createPilar(x, dimX + 1);
		}

		for (int y = 0; y < dimX + 2; y++) {
			map[0][y] = (createPilar(0, y));
			map[dimY + 1][y] = (createPilar(dimY + 1, y));
		}

		for (int x = 1; x < dimX + 1; x++) {
			for (int y = 1; y < dimY + 1; y++) {
				if (x % 2 == 1 || y % 2 == 1) {
					// Create random Bricks:
					if (!dist(mt)) {
						map[y][x] = new BrickWall(Pillars_Balls, x * sizeColliderPillar, y * sizeColliderPillar);
						wallEntities.push_back(map[y][x]);
					}
				}
				else {
					map[y][x] = createPilar(y, x);
				}
			}
		}
	}

	void update() {
		auto it = wallEntities.begin();
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != wallEntities.end()) {
			// Update the entities.
			(*it)->update();
			if ((*it)->expiredEntity) {

				// Remove the entity from the list of entities if it expired.
				delete(*it);
				it = wallEntities.erase(it);
			}
			else {
				++it;
			}
		}
	}

	bool circIntersect(Entity& e1, Entity& e2) {
		sf::FloatRect shape1 = e1.getGlobalBounds();
		sf::FloatRect shape2 = e2.getGlobalBounds();

		float dx = (shape1.left + shape1.width / 2) - (shape2.left + (shape2.width / 2));
		float dy = (shape1.top + (shape1.width / 2)) - (shape2.top + (shape2.width / 2));
		float distance = std::sqrt((dx * dx) + (dy * dy));

		return (distance <= (shape1.width / 2) + (shape2.width / 2));
	}

	bool intersectsCircleRect(Entity& e1circle, Entity& e2rect, double& x, double& y) {
		sf::FloatRect circle = e1circle.getGlobalBounds();
		sf::FloatRect rect = e2rect.getGlobalBounds();

		sf::Vector2f circleDistance;
		x = (circle.left + circle.width / 2) - (rect.left + rect.width / 2);
		y = (circle.top + circle.width / 2) - (rect.top + rect.height / 2);
		circleDistance.x = abs(x);
		circleDistance.y = abs(y);

		if (circleDistance.x > (rect.width / 2 + circle.width)) { return false; }
		if (circleDistance.y > (rect.height / 2 + circle.width)) { return false; }

		float moduloCentro = sqrtf(x * x + y * y);
		x /= moduloCentro;
		y /= moduloCentro;

		x /= 1000.0;
		y /= 1000.0;

		if (circleDistance.x <= (rect.width / 2)) { return true; }
		if (circleDistance.y <= (rect.height / 2)) { return true; }

		double cornerDistance_sq;
		cornerDistance_sq = pow((circleDistance.x - rect.width / 2), 2) +
			pow((circleDistance.y - rect.height / 2), 2);

		return (cornerDistance_sq <= pow(circle.width, 2));
	}

	void checkAndFixCollisions(Entity& e, std::vector<Entity*>& liveEntities) {

		for (std::vector<Entity*>& v : map) {
			for (Entity*& _e : v) {
				if (_e != nullptr && _e->expiredEntity) {
					_e = nullptr;
					continue;
				}

				if (_e != nullptr && e.collision(*_e)) {


					//calcular centro

					//sf::Vector2f position = e.getPosition();


					//if (_e->getGlobalBounds().contains(position))
					//{
					//}
					//else if (_e->getGlobalBounds().contains(position.x, position.y + e.getGlobalBounds().width))
					//{
					//	position.y = position.y + e.getGlobalBounds().width;
					//}
					//else if (_e->getGlobalBounds().contains(position.x + e.getGlobalBounds().height, position.y))
					//{
					//	position.x + e.getGlobalBounds().height;
					//}
					//else if (_e->getGlobalBounds().contains(position.x + e.getGlobalBounds().height, position.y + e.getGlobalBounds().width))
					//{
					//	position.x + e.getGlobalBounds().height;
					//	position.y = position.y + e.getGlobalBounds().width;
					//}

					/**/

					/*sf::Vector2f positionC = e.getPosition();

					positionC.x += (e.getGlobalBounds().width / 2);
					positionC.y += (e.getGlobalBounds().height / 2);
					sf::Vector2f positionCollision = _e->getPosition();
					positionCollision.x += (_e->getGlobalBounds().width / 2);
					positionCollision.y += (_e->getGlobalBounds().height / 2);
					sf::Vector2f t(positionC.x - positionCollision.x, positionC.y - positionCollision.y);
					float moduloCentro = sqrtf(t.x * t.x + t.y * t.y);
					t = t / moduloCentro;
					while (e.getGlobalBounds().intersects(_e->getGlobalBounds())) {
						e.setPosition(e.getPosition().x + t.x / 100, e.getPosition().y + t.y / 100);
					}*/

					double x;
					double y;
					while (intersectsCircleRect(e, *_e, x, y)) {
						e.setPosition(e.getPosition().x + x, e.getPosition().y + y);
					}
				}
			}
		}

		for (Entity*& _e : liveEntities) {
			if (_e != nullptr && _e->expiredEntity) {
				_e = nullptr;
				continue;
			}

			if (_e != nullptr && e.collision(*_e)) {
				double x;
				double y;
				
				Bomb* b;
				Fire* f;
				if ((b = dynamic_cast<Bomb*>(_e)) != nullptr) {
					while (intersectsCircleRect(e, *_e, x, y)) {
						e.setPosition(e.getPosition().x + x, e.getPosition().y + y);
					}
				} else if((f = dynamic_cast<Fire*>(_e)) != nullptr)
				{
					e.expiredEntity = true;
				}
			}
		}
		
		//std::cout << "--------------" << std::endl;
	}

	sf::Vector2i getMapCoordinates(int x, int y) {
		return sf::Vector2i((int)x / sizeColliderPillar, (int)y / sizeColliderPillar);
	}

	sf::Vector2i getMapCoordinates(sf::Vector2f pos) {
		return getMapCoordinates(pos.x, pos.y);
	}


	sf::Vector2f getMapCellCorner(sf::Vector2f pos) {
		return sf::Vector2f((int)pos.x / sizeColliderPillar * sizeColliderPillar, (int)pos.y / sizeColliderPillar * sizeColliderPillar);
	}

	Entity* getCellObject(sf::Vector2i pos) {
		return map[pos.y][pos.x];
	}

	void Draw(sf::RenderWindow& w) {
		w.draw(floor);
		for (std::vector<Entity*>& v : this->map) {
			for (Entity*& e : v) {
				if (e != nullptr) {
					w.draw(*e);
				}
			}
		}
	}
};