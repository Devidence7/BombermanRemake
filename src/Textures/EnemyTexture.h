#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Include/global.hpp"

#include "TextureVector.h"

class EnemyTexture : public TextureVector {

public:
	sf::Vector2f frameSize; // Size of the Entity Sprite

	EnemyTexture();

	sf::IntRect& getIdleSprite(int lookat, EnemyType enemyType);

	sf::IntRect& getWalkingSprite(LookingAtBi looking, int i, EnemyType enemyType);

    sf::IntRect& getDeathSprite(int i, EnemyType enemyType);
};
