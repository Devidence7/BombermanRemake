#pragma once
#include "TextureVector.h"
#include <SFML/Graphics.hpp>


class TeleporterTexture : public TextureVector {

public:
	TeleporterTexture();

	sf::IntRect& getFrame(int i, int state);
};

