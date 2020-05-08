#pragma once

#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"


class PowerUpTexture : public TextureVector {

public:
	PowerUpTexture();

	sf::IntRect& getFrame(int x, int y);
	sf::IntRect& getFrame(int x);
};