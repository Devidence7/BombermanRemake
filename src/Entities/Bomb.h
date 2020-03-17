#pragma once
#include <iostream>
#include "../Textures/BombTexture.h"
#include "../Textures/FireTexture.h"
#include "../Textures/TextureStorage.h"
#include "../Logic/Time.h"
#include "Entity.h"

class Bomb : public Entity {
public:
	// Gameplay Variables:
	int bombPower;

	// Texture Variables:
	BombTexture* bombTexture;
	double spriteCounter;
	const double spriteSpeed = 0.2;
	int actualFrame = 0;
	const int spriteFrames = 4;

	double explosionCounter;
	const double explosionTime = 1.35;

	Bomb(int bombPower) : Entity() {
		this->bombPower = bombPower;
		isFireDestroyable = true;
		fireCanGoThroght = true;

		explosionCounter = GameTime::getTimeNow();
		spriteCounter = GameTime::getTimeNow();

		// Texture Controller:
		bombTexture = &TextureStorage::getBombTexture();
		// Set starting sprite
		setTextureRect(bombTexture->getFrame(0));
		// Set sprite Sheet texture
		setTexture(bombTexture->getTexture());
	}

	void update() {
		// If it is time to explote:
		if (GameTime::getTimeNow() - explosionCounter > explosionTime) {
			expiredEntity = true;
		}

		if (GameTime::getTimeNow() - spriteCounter > spriteSpeed) {
			actualFrame = (actualFrame + 1) % spriteFrames;
			setTextureRect(bombTexture->getFrame(actualFrame));
			spriteCounter = GameTime::getTimeNow();
		}
	}
};


class Fire : public Entity {
public:
	FireTexture* fireTexture;

	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int spriteFrames;

	int explosionCounter;
	int explosionType;

	Fire(int type = 0) : Entity() {
		spriteCounter = 0;
		spriteSpeed = 5;
		actualFrame = 0;
		spriteFrames = 5;
		explosionCounter = 0;
		explosionType = type;

		// Texture Controller:
		fireTexture = &TextureStorage::getFireTexture();
		// Set starting sprite
		setTextureRect(fireTexture->getFrame(0, type));
		// Set sprite Sheet texture
		setTexture(fireTexture->getTexture());
	}

	void update() override{
		explosionCounter++;
		if (explosionCounter >= 25) {
			expiredEntity = true;
		}

		spriteCounter++;
		spriteCounter %= spriteSpeed;
		if (spriteCounter == 0) {
			actualFrame = (actualFrame + 1) % spriteFrames;
			setTextureRect(fireTexture->getFrame(actualFrame, explosionType));
		}
	}
};

