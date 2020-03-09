#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Textures/PlayerTexture.hpp"
#include "Entity.h"

class PlayerEntity : public Entity
{
public:
	unsigned int lifes;
	unsigned int bombsTimeLimit;
	float speedBoost;

	PlayerTexture entityTexture;
	LookingAt lastMovement; // Save last looked direction

	/*
	Constructor of Entity
	*/
	PlayerEntity() : Entity(new Collider2d(sf::Vector2f(0,0), sf::FloatRect(0,0,48,48), false, true))
	{
		lifes = 3;
		baseSpeed = 1;
		speedBoost = 5;
		velocity.x = 0;
		velocity.y = 0;

		lastMovement = LookingAt::down;
		// Set starting sprite
		setTextureRect(entityTexture.getIdleSprite(lastMovement));
		// Set sprite Sheet texture
		setTexture(entityTexture.getTexture());
	}
	PlayerEntity(Collider2d &col) : Entity(col)
	{
		// Speed
		lifes = 3;
		baseSpeed = 1;
		speedBoost = 5;
		velocity.x = 0;
		velocity.y = 0;

		lastMovement = LookingAt::down;

		// Set starting sprite
		setTextureRect(entityTexture.getIdleSprite(lastMovement));
		// Set sprite Sheet texture
		setTexture(entityTexture.getTexture());
	}

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity)
	{
		if (velocity.x == 0 && velocity.y == 0)
		{
			// If there is not speed set idle sprite
			setTextureRect(entityTexture.getIdleSprite(lastMovement));
		}
		else
		{
			// If there is speed we must update animation sprite every X time
			if (entityTexture.mustChangeSprite())
			{
				setTextureRect(entityTexture.getWalkingSprite(lastMovement));
			}
		}
	}

	sf::FloatRect getGlobalBounds() const override
	{
		//if(this->body != nullptr){
		//	return body->getRect();
		//}
		sf::Vector2f p = getPosition();
		float h = sf::Sprite::getGlobalBounds().height;
		return sf::FloatRect(p.x, p.y+(h/2), sf::Sprite::getGlobalBounds().width, (h/2) );
		
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
		move(velocity.x, velocity.y);

		if (playerBOMB)
		{
			return true;
		}
		return false;
	}
};