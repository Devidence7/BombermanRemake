#include "WallTexture.h"

WallTexture::WallTexture()
{
	if (!this->texture.loadFromFile("../textures/Entities/blocks.png"))
	{
		throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/blocks.png");
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 8; i++) {
			this->frames.push_back(sf::IntRect(51 * i, sizePillar * j, sizePillar, sizePillar));
		}
	}
	
}

sf::IntRect &WallTexture::getDefaultIntRect()
{
	return frames[1];
}

sf::IntRect &WallTexture::getRectWall(int i, int stage)
{
	return this->frames[i + 8 * stage];
}

sf::IntRect &WallTexture::getRectPillar(int stage)
{
	return this->frames[0 + 8 * stage];
}
