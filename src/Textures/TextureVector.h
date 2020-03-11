#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class TextureVector {
protected:
	std::vector<sf::IntRect> frames; // Frames of the Entity animation
	sf::Texture texture; // Entity Texture

public:
	sf::Texture& getTexture() {
		return texture;
	}

	virtual sf::IntRect& getDefaultIntRect() {
		return frames[0];
	}
};