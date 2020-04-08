#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"


class FireTexture : public TextureVector {

public:
	FireTexture();

	sf::IntRect& getFrame(int stage, int i);
};