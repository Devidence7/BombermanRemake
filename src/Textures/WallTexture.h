#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"

#define sizeWall 48
#define sizePillar 48

class WallTexture : public TextureVector {
	public:
	WallTexture();

	sf::IntRect& getDefaultIntRect() override;

	sf::IntRect &getRectWall(int i, int stage);

	sf::IntRect &getRectPillar(int stage);
};
