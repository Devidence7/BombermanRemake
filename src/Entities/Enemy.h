#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"

class EnemyEntity : public Entity {
public:
	/* 
	Constructor of Entity
	*/
    bool primerMov;
	EnemyEntity() : Entity() {
		// Speed
		speedBoost = 3;
		velocity.x = 1;
		velocity.y = 0;

		// Initialize sprite counters
		walkCounter = 0;
		walkFrames = 4;
		idleCounter = 0;
		idleFrames = 1;
		walkSpeed = 8;
		idleSpeed = 10;

		lastMovement = 0;

		frameSize.x = 48;
		frameSize.y = 54;
        primerMov=true;

		// Get texture sprites:
		if (!texture.loadFromFile("../textures/Entities/enemigoDefinitivo.png")) {
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
		
		

		if (primerMov) {
            std::cout <<"Primer mov \n";
			velocity.y = speedBoost;
			primerMov=false;
		}

		// Collision test
		std::cout << getGlobalBounds().top << " * " << velocity.y << std::endl;
		if (getGlobalBounds().top <= 0 && velocity.y < 0) {
			velocity.y = -velocity.y;
           // lastMovement=1;
           
		}
		if (getGlobalBounds().top + frameSize.y >= 600 && velocity.y > 0) {
			velocity.y = -velocity.y;
           // lastMovement=0;
		}
		if (getGlobalBounds().left <= 0 && velocity.x < 0) {
			velocity.x = -velocity.x;
            lastMovement=0;
           
		}
		if (getGlobalBounds().left + frameSize.x >= 800 && velocity.x > 0) {
			velocity.x = -velocity.x;
            lastMovement=1;
		}

		// Call animate function to change current sprite if needed.
		animate(velocity);

		// Move Entity position
		move(velocity.x, velocity.y);
	}
};