#pragma once
#include <SFML/Graphics/Rect.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"


class BombTexture : public TextureVector {

public:
	BombTexture();

	sf::IntRect& getFrame(int i);
};
