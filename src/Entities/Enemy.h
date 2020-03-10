#pragma once
#include <SFML/Graphics/Rect.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "../Textures/EnemyTexture.h"



class EnemyEntity : public Entity {

public:
	EnemyTexture enemyTexture;
	int lastMovement;
	int spriteSpeed;
	int spriteCounter;
	int actualFrame;
	int spriteFrames;
	int deathCounter;
	bool firstMov;
	EnemyEntity():Entity(new Collider2d(sf::Vector2f(0,0), sf::FloatRect(0,0,48,48), false, true)){
		velocity.x = 1;
		velocity.y = 0;
		lastMovement=0;
		spriteSpeed=5;
		spriteFrames=4;
		spriteCounter=0;
		actualFrame=0;
		baseSpeed=2;
		deathCounter=0;
		firstMov=true;
		move(500, 100);
		setTextureRect(enemyTexture.getWalkingSprite(0));
		// Set sprite Sheet texture
		setTexture(enemyTexture.getTexture());
	}

	void animate(sf::Vector2f velocity)
	{
	
		
			// If there is speed we must update animation sprite every X time
			
				setTextureRect(enemyTexture.getWalkingSprite(lastMovement));
		
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
	
	void update() {
		if (firstMov) {
            std::cout <<"Primer mov \n";
			velocity.x = baseSpeed;
			firstMov=false;
		}

		if (deathCounter >= 25) {
			expiredEntity = true;
		}
			
		if (getGlobalBounds().top <= 0 && velocity.y < 0) {
			velocity.y = -velocity.y;
           // lastMovement=1;
           
		}
		if (getGlobalBounds().top + enemyTexture.frameSize.y >= 600 && velocity.y > 0) {
			velocity.y = -velocity.y;
           // lastMovement=0;
		}
		if (getGlobalBounds().left <= 0 && velocity.x < 0) {
			velocity.x = -velocity.x;
            lastMovement=0;
           
		}
		if (getGlobalBounds().left + enemyTexture.frameSize.x >= 800 && velocity.x > 0) {
			velocity.x = -velocity.x;
            lastMovement=3;
		}

		// Call animate function to change current sprite if needed.
		spriteCounter++;
		spriteCounter %= spriteSpeed;
		if (spriteCounter == 0) {
			actualFrame = (actualFrame + 1) % spriteFrames;
			setTextureRect(enemyTexture.getFrame(lastMovement,actualFrame));
		}
		// Move Entity position
		move(velocity.x, velocity.y);
	}



};
