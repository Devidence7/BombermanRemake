#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Textures/PlayerTexture.hpp"
#include "Entity.h"

class PlayerEntity : public Entity
{
public:
	unsigned int lifes;
	unsigned int bombsTimeLimit = 30;
	int actualframe = 0;
	int frameSpeed = 10;
	int frameCounter = 0;
	float speedBoost = 1;

	PlayerTexture *playerTexture;
	LookingAt lastMovement; // Save last looked direction
	
	int	animationCounter{},
		walkFrames,  // Number of walking sprites
		walkCounter,  // Number of walking sprites
		idleFrames, // Number of idleling sprites
		idleCounter, // Number of idleling sprites
		walkSpeed,	// Number of ticks between walking sprites
		idleSpeed; // Number of ticks between idleling sprites

	/*
	Constructor of Entity
	*/
	PlayerEntity(PlayerTexture &pt) : Entity(), playerTexture(&pt)
	{
		lifes = 3;
		baseSpeed = 1;
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

		lastMovement = LookingAt::down;

		// TODO: Remove this
		move(100, 100);
		// Set starting sprite
		setTextureRect(playerTexture->getIdleSprite(lastMovement));
		// Set sprite Sheet texture
		setTexture(playerTexture->getTexture());
	}


	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity)
	{
		// If the player has died:
		if(!expiredEntity)
		{
			if (velocity.x == 0 && velocity.y == 0) {
				// If there is not speed set idle sprite
				setTextureRect(playerTexture->getIdleSprite(lastMovement));
			}
			else {
				// If there is speed we must update animation sprite every X time
				if (frameCounter == 0) {
					setTextureRect(playerTexture->getMoveSprite(lastMovement,actualframe));
					actualframe = (actualframe + 1) % 4;
				}
				frameCounter = (frameCounter + 1) % frameSpeed;
			}
		}
		else
		{
			if (frameCounter == 0) {
				setTextureRect(playerTexture->getDeathSprite(actualframe));
				actualframe = (actualframe + 1) % 7;
			}
			frameCounter = (frameCounter + 1) % frameSpeed;
		}
	}

	sf::FloatRect getGlobalBounds() const override
	{
		sf::FloatRect dim = sf::Sprite::getGlobalBounds();
		return sf::FloatRect(dim.left + (3 * dim.width / 8), dim.top + dim.height - dim.width / 4, dim.width / 4, dim.height / 4);
	}

	sf::Vector2f getCenterPosition()
	{
		sf::FloatRect p = getGlobalBounds();
		return sf::Vector2f(p.left + p.width/2, p.top + p.height / 2);
	}

	void update()
	{
		return;
	}

	/**
	 * 
	 */ 


	/*
	 * Update player position.
	 */
	bool updatePlayer()
	{
		// Player movement
		bool playerRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
		bool playerLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
		bool playerUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
		bool playerDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));

		bool playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

		// TODO: Remove this
		if (playerBOMB)
		{
			if (bombsTimeLimit < 30)
			{
				playerBOMB = false;
			}
			else
			{
				bombsTimeLimit = 0;
			}
		}
		bombsTimeLimit++;

		velocity.x = 0;
		velocity.y = 0;

		if (playerDown)
		{
			velocity.y = baseSpeed * speedBoost;
			lastMovement = LookingAt::down;
		}
		if (playerUp)
		{
			velocity.y = -baseSpeed * speedBoost;
			lastMovement = LookingAt::up;
		}
		if (playerLeft)
		{
			velocity.x = -baseSpeed * speedBoost;
			lastMovement = LookingAt::left;
		}
		if (playerRight)
		{
			velocity.x = baseSpeed * speedBoost;
			lastMovement = LookingAt::right;
		}

		// Call animate function to change current sprite if needed.
		animate(velocity);

		// Move Entity position
		/*if(!expiredEntity)
		{*/
			move(velocity.x, velocity.y);
		//}
		

		if (playerBOMB)
		{
			return true;
		}
		return false;
	}
};