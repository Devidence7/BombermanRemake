#pragma once
#include "Entity.h"
#include "../Textures/PowerUpTexture.h"
#include "../Textures/TextureStorage.h"

class PowerUp : public Entity {
public:
	PowerUpTexture* powerUpTexture;
	PowerUpType powerUpType;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int spriteFrames;

	int explosionCounter;
	PowerUp() : Entity() {
		spriteCounter = 0;
		spriteSpeed = 15;
		actualFrame = 0;
		spriteFrames = 3;
		explosionCounter = 0;

		// Texture Controller:
		powerUpTexture = &TextureStorage::getPowerUpTexture();
		// Set sprite Sheet texture
		setTexture(powerUpTexture->getTexture());
	}


	void update() {

	}
};

class MoreFirePowerUp : public PowerUp {
	MoreFirePowerUp() {
		powerUpType = moreFire;
		// Set starting sprite
		setTextureRect(powerUpTexture->getFrame(3, 0));
	}
};