#pragma once
#include <iostream>
#include "../Textures/BombTexture.h"
#include "Entity.h"

class Bomb : public Entity {
public:
	BombTexture *entityTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int spriteFrames;
	
	Bomb(BombTexture &bt)
	{
		spriteCounter = 0;
		spriteSpeed = 15;
		actualFrame = 0;
		spriteFrames = 3;
		
		// Texture Controller:
		entityTexture = &bt;
		// Set starting sprite
		setTextureRect(entityTexture->getFrame(0));
		// Set sprite Sheet texture
		setTexture(entityTexture->getTexture());
	}

	void Entity::Eat() override
	{
		std::cout << "Hey" << std::endl;
		spriteCounter++;
		spriteCounter %= spriteSpeed;
		if (spriteCounter == 0) {
			actualFrame = (actualFrame + 1) % spriteFrames;
			setTextureRect(entityTexture->getFrame(actualFrame));
		}
	}
};
