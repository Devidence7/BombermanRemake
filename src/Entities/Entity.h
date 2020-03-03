#pragma once
#include <SFML/Graphics.hpp>

class Entity : public sf::Sprite {
public:
	float speedBoost; // Base Entity Speed (TODO Make baseSpeed parameter too)
	sf::Vector2f velocity; // Speed of the Entity right now.
	sf::Vector2f size; // Size of the Entity
	sf::Vector2f frameSize; // Size of the Entity Sprite

	std::vector<sf::IntRect> frames; // Frames of the Entity animation

	sf::Texture texture; // Entity Texture

	int	animationCounter,
		walkFrames,  // Number of walking sprites
		walkCounter,  // Number of walking sprites
		idleFrames, // Number of idleling sprites
		idleCounter, // Number of idleling sprites
		walkSpeed,	// Number of ticks between walking sprites
		idleSpeed; // Number of ticks between idleling sprites

	int lastMovement; // Save last looked direction

	/* 
	*  Constructor of Entity
	*/
	Entity() : Sprite() {

	}

};