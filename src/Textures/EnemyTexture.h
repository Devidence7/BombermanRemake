#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextureVector.h"

class EnemyTexture : public TextureVector {

public:
	

	sf::Vector2f frameSize; // Size of the Entity Sprite
	int	animationCounter{},
		walkFrames,  // Number of walking sprites
		walkCounter,  // Number of walking sprites
		idleFrames, // Number of idleling sprites
		idleCounter, // Number of idleling sprites
		walkSpeed,	// Number of ticks between walking sprites
		idleSpeed; // Number of ticks between idleling sprites

	EnemyTexture() {
		frameSize.x = 48;
		frameSize.y = 54;

		// Initialize sprite counters
		walkCounter = 0;
		walkFrames = 4;
		idleCounter = 0;
		idleFrames = 1;
		walkSpeed = 8;
		idleSpeed = 10;

		// Get texture sprites:
		if (!this->texture.loadFromFile("../textures/Entities/enemigoDefinitivo.png")) {
			std::cerr << "Unable to load texture:" << std::endl;
			std::cerr << "Texture location: ../textures/Entities/enemigoDefinitivo.png" << std::endl;
		}

		// Save sprites positions in a array
		for (int y = 0; y < 1; y++) {
			for (int x = 0; x < 8; x++) {
				sf::IntRect frame(frameSize.x * x, frameSize.y * y, frameSize.x, frameSize.y);
				frames.push_back(frame);
			}
		}
	}

	sf::IntRect& getIdleSprite(int lookat) {
		return frames[lookat * 4];
	}


	sf::IntRect& getWalkingSprite(int lookat) {
		walkCounter = (walkCounter + 1) % walkFrames;
		return frames[walkCounter + 4 * lookat];
	}

	bool mustChangeSprite() {
		// If there is speed set walking sprites
		animationCounter++;
		animationCounter %= walkSpeed;
		return (animationCounter == 0);
	}

	sf::IntRect& getFrame(int lastMovement,int i) {
		return this->frames[lastMovement+i];
	}


    sf::IntRect& getDeathSprite(int lookat) {
		walkCounter = (walkCounter + 1) % walkFrames;
		return frames[walkCounter + 4 * lookat];
	}

};
