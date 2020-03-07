#pragma once
#include <SFML/Graphics.hpp>
#include "../Textures/PlayerTexture.hpp"
#include "Entity.h"

class PlayerEntity : public Entity {
public:
	unsigned int lifes;
	unsigned int bombsTimeLimit;
	float speedBoost;

	PlayerTexture entityTexture;
	PlayerTexture::LookingAt lastMovement; // Save last looked direction

	/*
	Constructor of Entity
	*/
	PlayerEntity() {
		// Speed
		lifes = 3;
		baseSpeed = 1;
		speedBoost = 5;
		velocity.x = 0;
		velocity.y = 0;

		lastMovement = PlayerTexture::down;

		// Texture Controller:
		entityTexture = PlayerTexture();
		// Set starting sprite
		setTextureRect(entityTexture.getIdleSprite(lastMovement));
		// Set sprite Sheet texture
		setTexture(entityTexture.getTexture());
	}

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity) {
		if (velocity.x == 0 && velocity.y == 0) {
			// If there is not speed set idle sprite
			setTextureRect(entityTexture.getIdleSprite(lastMovement));
		}
		else {
			// If there is speed we must update animation sprite every X time
			if (entityTexture.mustChangeSprite()) {
				setTextureRect(entityTexture.getWalkingSprite(lastMovement));
			}
		}
	}

	void Entity::update() override {
		return;
	}


	/*
	 * Update player position.
	 */
	bool updatePlayer() {
		// Player movement
		bool playerRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
		bool playerLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
		bool playerUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
		bool playerDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));

		bool playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

		// TODO: Remove this
		if (playerBOMB) {
			if (bombsTimeLimit < 30) {
				playerBOMB = false;
			}
			else {
				bombsTimeLimit = 0;
			}
		}
		bombsTimeLimit++;
		
		velocity.x = 0;
		velocity.y = 0;

		if (playerDown) {
			velocity.y = baseSpeed * speedBoost;
			lastMovement = PlayerTexture::down;
		}
		if (playerUp) {
			velocity.y = -baseSpeed * speedBoost;
			lastMovement = PlayerTexture::up;
		}
		if (playerLeft) {
			velocity.x = -baseSpeed * speedBoost;
			lastMovement = PlayerTexture::left;
		}
		if (playerRight) {
			velocity.x = baseSpeed * speedBoost;
			lastMovement = PlayerTexture::right;
		}

		// Call animate function to change current sprite if needed.
		animate(velocity);

		// Move Entity position
		move(velocity.x, velocity.y);

		if (playerBOMB) {
			return true;
		}
		return false;
	}
};