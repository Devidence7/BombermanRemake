#include <vector>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "../Textures/WallTexture.h"
#include "wall.hpp"
#include "../Entities/mapSprites.hpp"

class Map
{
	WallTexture &Pillars_Balls;
	std::vector<std::vector<Entity *>> map;
	sf::RectangleShape floor;

public:
	Map(WallTexture &bw) : Pillars_Balls(bw)
	{
		//Suelo
		map = std::vector<std::vector<Entity *>>();

		floor.setSize(sf::Vector2f(1600, 1200));
		floor.setFillColor(sf::Color(0, 100, 0));
		//Crear pilares
		for (int i = 0; i < 15; i++)
		{
			map.push_back(std::vector<Entity *>(25));
			for (int j = 0; j < 25; j++)
			{
				if (i % 2 == 0 || j % 2 == 0)
				{
					map[i].push_back(nullptr);
				}
				else
				{
					Pillar *p = new Pillar((sizeColliderPillar + 5)*i, j*(sizeColliderPillar + 5));
					p->setTexture(Pillars_Balls.getTexture());
					p->setTextureRect(Pillars_Balls.getRectPillar());
					p->setPosition(sf::Vector2f((sizeColliderPillar + 5)*i, j*(sizeColliderPillar + 5)));
					map[i].push_back(p);
				}
			}
		}
	}

	void checkAndFixCollisions(Entity &e)
	{

		for (std::vector<Entity *> &v : map)
		{
			for (Entity *&_e : v)
			{
				if (_e != nullptr && e.collision(*_e))
				{
					//calcular centro

					//sf::Vector2f position = e.getPosition();
					sf::Vector2f positionC = e.getPosition();

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

					positionC.x += (e.getGlobalBounds().width / 2);
					positionC.y += (e.getGlobalBounds().height / 2);
					sf::Vector2f positionCollision = _e->getPosition();
					positionCollision.x += (_e->getGlobalBounds().width / 2);
					positionCollision.y += (_e->getGlobalBounds().height / 2);
					sf::Vector2f t(positionC.x - positionCollision.x, positionC.y - positionCollision.y);
					float moduloCentro = sqrtf(t.x*t.x + t.y * t.y);
					t = t/moduloCentro;
					while (e.getGlobalBounds().intersects(_e->getGlobalBounds()))
					{
						e.setPosition(e.getPosition().x + t.x, e.getPosition().y + t.y);
					}
					
				}
			}
		}
	}

	void Draw(sf::RenderWindow &w)
	{
		w.draw(floor);
		for (std::vector<Entity *> &v : this->map)
		{
			for (Entity *&e : v)
			{
				if (e != nullptr)
				{
					w.draw(*e);
				}
			}
		}
	}
};