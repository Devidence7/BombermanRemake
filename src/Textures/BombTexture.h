#pragma once
#include <SFML/Graphics/Rect.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"


class BombTexture : public TextureVector {

public:
	BombTexture() {
		if (!this->texture.loadFromFile("../textures/Entities/bombs.png")) {
			throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/bombs.png");
		}

		for (int i = 0; i < 3; i++) {
			this->frames.push_back(sf::IntRect(0 + 51 * i, 0, 48, 48));
		}
		this->frames.push_back(sf::IntRect(0 + 51 * 1, 0, 48, 48));
	}

	sf::IntRect& getFrame(int i) {
		return frames[i];
	}
};
