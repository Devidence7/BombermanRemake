#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Texture {
	std::vector<sf::IntRect> frames; // Frames of the Entity animation
	sf::Texture texture; // Entity Texture
	
};