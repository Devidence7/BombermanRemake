#include "PlayerTexture.hpp"

	
	PlayerTexture::PlayerTexture() {
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

	sf::IntRect& PlayerTexture::getIdleSprite(LookingAt lookat) {
		return frames[lookat * 4];
	}

	sf::IntRect& PlayerTexture::getMoveSprite(LookingAt lookat, int i) {
		return frames[lookat * 4 + i];
	}

	sf::IntRect& PlayerTexture::getDeathSprite(int i) {
		return frames[44 + i];
	}
