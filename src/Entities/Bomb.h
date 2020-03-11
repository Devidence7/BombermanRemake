#pragma once
#include <iostream>
#include "../Textures/BombTexture.h"
#include "../Textures/FireTexture.h"
#include "Entity.h"

class Bomb : public Entity {
public:
	BombTexture* bombTexture;
	FireTexture* fireTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int spriteFrames;

	int explosionCounter;
	Bomb(BombTexture& bt, FireTexture& ft) : Entity(bt) {
		spriteCounter = 0;
		spriteSpeed = 15;
		actualFrame = 0;
		spriteFrames = 3;
		explosionCounter = 0;

		// Texture Controller:
		bombTexture = &bt;
		fireTexture = &ft;
		// Set starting sprite
		setTextureRect(bombTexture->getFrame(0));
		// Set sprite Sheet texture
		setTexture(bombTexture->getTexture());
	}

	void update() {
		explosionCounter++;
		if (explosionCounter >= 135) {
			expiredEntity = true;
		}

		spriteCounter++;
		spriteCounter %= spriteSpeed;
		if (spriteCounter == 0) {
			actualFrame = (actualFrame + 1) % spriteFrames;
			setTextureRect(bombTexture->getFrame(actualFrame));
		}
	}

	FireTexture* getFireTexture() {
		return fireTexture;
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

	Fire(FireTexture* ft, int type = 0) : Entity(*ft), fireTexture(ft) {
		spriteCounter = 0;
		spriteSpeed = 5;
		actualFrame = 0;
		spriteFrames = 5;
		explosionCounter = 0;
		explosionType = type;
	}

	void update() {
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

