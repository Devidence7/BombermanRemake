#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextureVector.h"

class PlayerTexture : public TextureVector {

public:
	// Size of the Entity Sprite
	const int FRAME_SIZE_X = 57;
	const int FRAME_SIZE_Y = 75;
	
	PlayerTexture() {
		if (!this->texture.loadFromFile("../textures/Entities/BombermanSpriteSheet.png")) {
			throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/BombermanSpriteSheet.png");
		}
		
		// Save sprites positions in a array
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				sf::IntRect frame(FRAME_SIZE_X * x, FRAME_SIZE_Y * y, FRAME_SIZE_X, FRAME_SIZE_Y);
				frames.push_back(frame);
			}
		}
		for (int y = 4; y < 9; y++) {
			for (int x = 0; x < 7; x++) {
				sf::IntRect frame(FRAME_SIZE_X * x, FRAME_SIZE_Y * y, FRAME_SIZE_X, FRAME_SIZE_Y);
				frames.push_back(frame);
			}
		}
	}

	sf::IntRect& getIdleSprite(LookingAt lookat) {
		return frames[lookat * 4];
	}

	sf::IntRect& getMoveSprite(LookingAt lookat, int i) {
		return frames[lookat * 4 + i];
	}

	sf::IntRect& getDeathSprite(int i) {
		return frames[44 + i];
	}
};
