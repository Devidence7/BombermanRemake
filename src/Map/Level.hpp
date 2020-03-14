#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <SFML/Graphics.hpp>
#include <memory>


#include "../Entities/Bomb.h"
#include "../Entities/BrickWall.h"
#include "../Textures/WallTexture.h"
#include "wall.hpp"
#include "../Entities/Pillar.hpp"
#include "../Include/global.hpp"
typedef std::shared_ptr<Entity> Entity_ptr;

/**
 *
 * Esta clase se encarga de gestionar la l�gica del juego
 *
 */
class Level {
	// All entities vector:
	std::vector<Entity_ptr> entities;

	// Level map:
	int dimY = 15;
	int dimX = 25;
	std::vector<std::vector<Entity_ptr>> map;
	sf::RectangleShape floor;

public:

	Level() {
		// Reserve space for faster insert, delete of the entities
		entities.reserve(10000);

		// Todo use Random class
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, 3);

		// Create map matrix:
		map = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));

		// Background:
		floor.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));
		floor.setFillColor(sf::Color(0, 100, 0));

		// Create all pillars:
		for (int x = 0; x < dimX + 2; x++) {
			addPillar(x, 0);
			addPillar(x, dimY + 1);
		}

		for (int y = 0; y < dimY + 2; y++) {
			addPillar(0, y);
			addPillar(dimX + 1, y);
		}

		for (int x = 1; x < dimX + 1; x++) {
			for (int y = 1; y < dimY + 1; y++) {
				if (x % 2 == 1 || y % 2 == 1) {
					// Create random Bricks:
					if (!dist(mt)) {
						addWall(x, y);
					}
				}
				else {
					addPillar(x, y);
				}
			}
		}
	}

	void addPillar(int x, int y) {
		map[y][x] = std::make_shared<Pillar>(Pillar(x, y));
		entities.push_back(Entity_ptr(map[y][x]));
	}

	void addWall(int x, int y) {
		map[y][x] = std::make_shared<BrickWall>(BrickWall(x, y));
		entities.push_back(Entity_ptr(map[y][x]));
	}

	void update() {
		auto it = entities.begin();
		int counter = 0;
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != entities.end()) {

			// Update the entities.
			(*it)->update();
			if ((*it)->expiredEntity) {

				// If it is a bomb
				std::shared_ptr<Bomb> b;
				if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr) {
					// Create fires
					createFires(*b);

					// When adding entities vector iterator can be invalidated:
					it = entities.begin() + counter;
				}

				// Remove the entity from the list of entities if it expired.
				it->reset();
				it = entities.erase(it);
			}
			else {
				++it;
				counter++;
			}
		}
	}

	void draw(sf::RenderWindow& w) {
		// Draw the floor:
		w.draw(floor);

		// Draw the entities
		for (auto e : entities) {
			w.draw(*e);
		}
	}

	void addEntity(Entity_ptr e) {
		entities.push_back(e);
	}

	void createFire(int type, int posX, int posY) {
		// If it is a bomb
		Entity_ptr e = getCellObject(getMapCoordinates(posX, posY));
		std::shared_ptr<BrickWall> bw;
		std::shared_ptr<Pillar> p;
		if ((bw = std::dynamic_pointer_cast<BrickWall>(e)) != nullptr) {
			;
			bw->isDestroyed = true;
		}
		else if (std::dynamic_pointer_cast<Pillar>(e) != nullptr) {
			// Do nothing
		}
		else {
			//Collider2d colFire(sf::Vector2f(0, 0), sf::FloatRect(0, 0, 48, 48), true);
			std::shared_ptr<Fire> f = std::make_shared<Fire>(Fire(type));
			f->setPosition(posX, posY);
			addEntity(f);
		}
	}

	void createFires(Bomb& b) {
		// Central:
		createFire(0, b.getPosition().x, b.getPosition().y);
		// Down:
		createFire(2, b.getPosition().x, b.getPosition().y + 48);
		// Up:
		createFire(3, b.getPosition().x, b.getPosition().y - 48);
		// Left:
		createFire(6, b.getPosition().x - 48, b.getPosition().y);
		// Right:
		createFire(5, b.getPosition().x + 48, b.getPosition().y);
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

	void checkAndFixCollisions(Entity& e) {
		for(Entity_ptr _e : entities){
			if (_e->expiredEntity) {
				_e = nullptr;
				continue;
			}

			if (e.collision(*_e)) {


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

	sf::Vector2i getMapCoordinates(int x, int y) {
		return sf::Vector2i((int)x / SIZE_PILLAR, (int)y / SIZE_PILLAR);
	}

	sf::Vector2i getMapCoordinates(sf::Vector2f pos) {
		return getMapCoordinates(pos.x, pos.y);
	}


	sf::Vector2f getMapCellCorner(sf::Vector2f pos) {
		return sf::Vector2f((int)pos.x / SIZE_PILLAR * SIZE_PILLAR, (int)pos.y / SIZE_PILLAR * SIZE_PILLAR);
	}

	Entity_ptr& getCellObject(sf::Vector2i pos) {
		return map[pos.y][pos.x];
	}
};