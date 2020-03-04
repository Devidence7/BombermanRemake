#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"

class PlayerEntity : public Entity {
public:
	/* 
	Constructor of Entity
	*/
	PlayerEntity() : Entity() {
		// Speed
		speedBoost = 5;
		velocity.x = 0;
		velocity.y = 0;

		// Initialize sprite counters
		walkCounter = 0;
		walkFrames = 4;
		idleCounter = 0;
		idleFrames = 1;
		walkSpeed = 8;
		idleSpeed = 10;

		lastMovement = 0;

		frameSize.x = 57;
		frameSize.y = 75;

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
		
		// Set starting sprite
		setTextureRect(frames[0]);
		// Set sprite Sheet texture
		setTexture(texture);
	}

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity) {
		if (velocity.x == 0 && velocity.y == 0) {
			// If there is not speed set idle sprite
			setTextureRect(frames[lastMovement * 4]);
		}
		else {
			// If there is speed set walking sprites
			animationCounter %= walkSpeed;
			if (animationCounter == 0) {
				walkCounter = (walkCounter + 1) % walkFrames;
				setTextureRect(frames[walkCounter + 4 * lastMovement]);
			}
		}
		animationCounter++;
	}

	/*
	 * Update player position.
	 */
	void update() {
		// Player movement
		bool playerRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
		bool playerLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
		bool playerUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
		bool playerDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));

		velocity.x = 0;
		velocity.y = 0;

		if (playerDown) {
			velocity.y = speedBoost;
			lastMovement = 0;
		}
		if (playerUp) {
			velocity.y = -speedBoost;
			lastMovement = 1;
		}
		if (playerLeft) {
			velocity.x = -speedBoost;
			lastMovement = 2;
		}
		if (playerRight) {
			velocity.x = speedBoost;
			lastMovement = 3;
		}

		// Collision test
		std::cout << getGlobalBounds().top << " * " << velocity.y << std::endl;
		if (getGlobalBounds().top <= 0 && velocity.y < 0) {
			velocity.y = 0;
		}
		if (getGlobalBounds().top + frameSize.y >= 600 && velocity.y > 0) {
			velocity.y = 0;
		}
		if (getGlobalBounds().left <= 0 && velocity.x < 0) {
			velocity.x = 0;
		}
		if (getGlobalBounds().left + frameSize.x >= 800 && velocity.x > 0) {
			velocity.x = 0;
		}

		// Call animate function to change current sprite if needed.
		animate(velocity);

		// Move Entity position
		move(velocity.x, velocity.y);
	}
};