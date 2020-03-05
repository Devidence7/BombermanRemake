#include <vector>
#include <SFML/Graphics.hpp>
#include "../Textures/WallTexture.h"
#include "wall.hpp"


class Map {
	WallTexture& Pillars_Balls;
	std::vector<WallBlock> active_walls;
	std::vector<sf::Sprite> pillars;
	sf::RectangleShape floor;

public:
	Map(WallTexture& bw) : Pillars_Balls(bw) {
		//Suelo
		floor.setSize(sf::Vector2f(1600, 1200));
		floor.setFillColor(sf::Color(0, 100, 0));
		//Crear pilares
		for (int i = 48; i <= 2 * 8 * 48; i = i + 2 * 48) {
			for (int j = 48; j <= 2 * 8 * 48; j = j + 2 * 48) {
				sf::Sprite p;
				p.setTexture(Pillars_Balls.getTexture());
				p.setTextureRect(Pillars_Balls.getRectPillar());
				p.setPosition(sf::Vector2f(i, j));
				pillars.push_back(p);
			}
		}
	}

	void Draw(sf::RenderWindow& w) {
		w.draw(floor);
		for (sf::Sprite& s : pillars) {
			w.draw(s);
		}
	}

};