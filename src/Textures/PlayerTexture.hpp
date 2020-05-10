#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Exceptions/ExceptionsGame.hpp"
#include "../Include/global.hpp"
#include "TextureVector.h"

class PlayerTexture : public TextureVector {

public:
	// Size of the Entity Sprite
	const int FRAME_SIZE_X = 57;
	const int FRAME_SIZE_Y = 75;
	
	PlayerTexture();
	sf::IntRect& getIdleSprite(LookingAt lookat);

	sf::IntRect& getMoveSprite(LookingAt lookat, int i);

	sf::IntRect& getIdleSpriteWithBomb(LookingAt lookat);

	sf::IntRect& getMoveSpriteWithBomb(LookingAt lookat, int i);

	sf::IntRect& getThrowBomb(LookingAt lookat, int i);

	sf::IntRect& getDeathSprite(int i);
};

class PlayerColor : public TextureVector {

public:
	// Size of the Entity Sprite
	const int FRAME_SIZE_X = 57;
	const int FRAME_SIZE_Y = 75;

	PlayerColor();
	sf::IntRect& getIdleSprite(LookingAt lookat);

	sf::IntRect& getMoveSprite(LookingAt lookat, int i);

	sf::IntRect& getDeathSprite(int i);
};
