#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextureVector.h"

class PlayerTexture : public TextureVector {

public:
	

	sf::Vector2f frameSize; // Size of the Entity Sprite
	int	animationCounter{},
		walkFrames,  // Number of walking sprites
		walkCounter,  // Number of walking sprites
		idleFrames, // Number of idleling sprites
		idleCounter, // Number of idleling sprites
		walkSpeed,	// Number of ticks between walking sprites
		idleSpeed; // Number of ticks between idleling sprites

	PlayerTexture() {
		frameSize.x = 57;
		frameSize.y = 75;

		// Initialize sprite counters
		walkCounter = 0;
		walkFrames = 4;
		idleCounter = 0;
		idleFrames = 1;
		walkSpeed = 8;
		idleSpeed = 10;

		// Get texture sprites:
		if (!texture.loadFromFile("../textures/Entities/BombermanSpriteSheet.png")) {
			std::cerr << "Unable to load texture:" << std::endl;
			std::cerr << "Texture location: ../textures/Entities/BombermanSpriteSheet.png" << std::endl;
		}

		// Save sprites positions in a array
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				sf::IntRect frame(frameSize.x * x, frameSize.y * y, frameSize.x, frameSize.y);
				frames.push_back(frame);
			}
		}
		for (int y = 4; y < 9; y++) {
			for (int x = 0; x < 7; x++) {
				sf::IntRect frame(frameSize.x * x, frameSize.y * y, frameSize.x, frameSize.y);
				frames.push_back(frame);
			}
		}
	}

	sf::IntRect& getIdleSprite(LookingAt lookat) {
		return frames[lookat * 4];
	}

	sf::IntRect& getMoveSprite(int i, int j) {
		return frames[i * 4 + j];
	}

	sf::IntRect& getDeathSprite(int i) {
		return frames[44 + i];
	}

	bool mustChangeSprite() {
		// If there is speed set walking sprites
		animationCounter++;
		animationCounter %= walkSpeed;
		return (animationCounter == 0);
	}

	sf::IntRect& getWalkingSprite(LookingAt lookat) {
		walkCounter = (walkCounter + 1) % walkFrames;
		return frames[walkCounter + 4 * lookat];
	}

};
