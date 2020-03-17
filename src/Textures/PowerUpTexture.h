#pragma once

#pragma once
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"


class PowerUpTexture : public TextureVector {

public:
	PowerUpTexture() {
		if (!this->texture.loadFromFile("../textures/Entities/PowerUps.png")) {
			throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/PowerUps.png");
		}

		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 10; x++) {
				this->frames.push_back(sf::IntRect(0 + 51 * x, 0 + 51 * y, 48, 48));
			}
		}
	}

	sf::IntRect& getFrame(int x, int y) {
		return frames[y * 10 + x];
	}
};