#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Textures/EnemyTexture.h"
#include "Entity.h"

class EnemyEntity : public Entity {
public:
	/* 
	Constructor of Entity
	*/
    bool primerMov;
	EnemyTexture *enemyTexture;
	int lastMovement; 
	EnemyEntity( EnemyTexture &et) : Entity() {
		// Speed
		baseSpeed = 3;
		velocity.x = 1;
		velocity.y = 0;

		// Initialize sprite counters

		lastMovement = 0;

        primerMov=true;

		move(600, 100);

		// Get texture sprites:

		
		
		// Set starting sprite
		setTextureRect(enemyTexture->getFrame(0));
		// Set sprite Sheet texture
		setTexture(enemyTexture->getTexture());
		
	}

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity) {
		/*if (velocity.x == 0 && velocity.y == 0) {
			// If there is not speed set idle sprite
			setTextureRect(enem);
		}*/
			// If there is speed set walking sprites
			setTextureRect(enemyTexture->getWalkingSprite(lastMovement));
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




	/*
	 * Update player position.
	 */
	void update() {
		// Player movement
		
		

		if (primerMov) {
            std::cout <<"Primer mov \n";
			velocity.y = baseSpeed;
			primerMov=false;
		}

		// Collision test
		std::cout << getGlobalBounds().top << " * " << velocity.y << std::endl;
		if (getGlobalBounds().top <= 0 && velocity.y < 0) {
			velocity.y = -velocity.y;
           // lastMovement=1;
           
		}
		if (getGlobalBounds().top + enemyTexture->frameSize.y >= 600 && velocity.y > 0) {
			velocity.y = -velocity.y;
           // lastMovement=0;
		}
		if (getGlobalBounds().left <= 0 && velocity.x < 0) {
			velocity.x = -velocity.x;
            lastMovement=0;
           
		}
		if (getGlobalBounds().left + enemyTexture->frameSize.x >= 800 && velocity.x > 0) {
			velocity.x = -velocity.x;
            lastMovement=1;
		}

		// Call animate function to change current sprite if needed.
		animate(velocity);

		// Move Entity position
		move(velocity.x, velocity.y);
	}
};