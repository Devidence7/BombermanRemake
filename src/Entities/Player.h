#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class PlayerEntity : public Entity {
public:
	/* 
	Constructor of Entity
	*/
	PlayerEntity(sf::Texture& t) : Entity() {
		// Speed
		speedBoost = 5;
		velocity.x = 0;
		velocity.y = 0;

		// Inicialize sprite counters
		walkCounter = 0;
		walkFrames = 2;
		idleCounter = 0;
		idleFrames = 1;
		walkSpeed = 8;
		idleSpeed = 10;

		lastMovement = 0;

		frameSize.x = 51;
		frameSize.y = 75;

		// Get texture sprites:
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 3; x++) {
				sf::IntRect frame(15 + frameSize.x * x, 24 + frameSize.y * y, frameSize.x, frameSize.y);
				frames.push_back(frame);
			}
		}
		
		// Set starting sprite
		setTextureRect(frames[0]);
		// Set sprite Sheet texture
		setTexture(t);
	}

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity) {
		if (velocity.x == 0 && velocity.y == 0) {
			// If there is not speed set idle sprite
			setTextureRect(frames[lastMovement * 3]);
		}
		else {
			// If there is speed set walking sprites
			animationCounter %= walkSpeed;
			if (animationCounter == 0) {
				walkCounter = (walkCounter + 1) % walkFrames;
				setTextureRect(frames[(1 + walkCounter) + 3 * lastMovement]);
			}
		}
		animationCounter++;
	}

	/*
	 * Update player position.
	 */
	void update(bool playerDown, bool playerUp, bool playerLeft, bool playerRight) {
		velocity.x = 0;
		velocity.y = 0;

		if (playerDown) {
			velocity.x = speedBoost;
			lastMovement = 0;
		}
		if (playerUp) {
			velocity.x = -speedBoost;
			lastMovement = 1;
		}
		if (playerLeft) {
			velocity.y = -speedBoost;
			lastMovement = 2;
		}
		if (playerRight) {
			velocity.y = speedBoost;
			lastMovement = 3;
		}

		// Call animate to change current sprite if needed.
		animate(velocity);

		// Move Entity position
		move(velocity.y, velocity.x);
	}
};