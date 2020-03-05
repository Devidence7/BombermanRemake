#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"


class FireTexture : public TextureVector {

public:
	FireTexture() {
		if (!this->texture.loadFromFile("../textures/fires.png")) {
			throw ExceptionLoadImage("Imagen no cargada: ../textures/fires.png");
		}

		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 35; x++) {
				this->frames.push_back(sf::IntRect(0 + 49 * x, 0 + 49 * y, 48, 48));
			}
		}
	}

	sf::Texture& getTexture() {
		return this->texture;
	}

	sf::IntRect& getFrame(int stage, int i) {
		return this->frames[7 * stage + i];
	}
};