#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Include/global.hpp"

#include "TextureVector.h"

class EnemyTexture : public TextureVector {

public:
	sf::Vector2f frameSize; // Size of the Entity Sprite

	EnemyTexture() {
		frameSize.x = 51;
		frameSize.y = 54;
		
		// Get texture sprites:
		if (!this->texture.loadFromFile("../textures/Entities/enemies.png")) {
			std::cerr << "Unable to load texture:" << std::endl;
			std::cerr << "Texture location: ../textures/Entities/enemies.png" << std::endl;
		}

		// Save sprites positions in a array
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 13; x++) {
				sf::IntRect frame((frameSize.x + 3) * x, frameSize.y * y, frameSize.x, frameSize.y);
				frames.push_back(frame);
			}
		}
	}

	sf::IntRect& getIdleSprite(int lookat, EnemyType enemyType) {
		return frames[TOTAL_ENEMY_FRAMES * enemyType + lookat * 4];
	}

	sf::IntRect& getWalkingSprite(LookingAtBi looking, int i, EnemyType enemyType) {
		return frames[TOTAL_ENEMY_FRAMES * enemyType + 4 * looking + i];
	}

    sf::IntRect& getDeathSprite(int i, EnemyType enemyType) {
		return frames[TOTAL_ENEMY_FRAMES * enemyType + 8 + i];
	}
};
