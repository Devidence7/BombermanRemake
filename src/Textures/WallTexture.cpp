#include "WallTexture.h"

WallTexture::WallTexture()
{
	if (!this->texture.loadFromFile("../textures/Entities/blocks.png"))
	{
		throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/blocks.png");
	}

	for (int i = 0; i < 8; i++)
	{
		this->frames.push_back(sf::IntRect(51 * i, 0, sizePillar, sizePillar));
	}
}

sf::IntRect &WallTexture::getDefaultIntRect()
{
	return frames[1];
}

sf::IntRect &WallTexture::getRectWall(int i)
{
	return this->frames[i];
}

sf::IntRect &WallTexture::getRectPillar()
{
	return this->frames[0];
}
