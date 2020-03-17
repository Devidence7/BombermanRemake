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
		isFireDestroyable = true;
		fireCanGoThroght = true;

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

	virtual void setPlayerStatus(PlayerEntity &pe) {
		throw NoImplementedException("Intentando ejecutar UPDATE Entity");
	}

};

class MoreFirePowerUp : public PowerUp {
public:
	MoreFirePowerUp(sf::Vector2f pos) : PowerUp() {
		powerUpType = moreFire;

		// Set position
		setPosition(pos);

		// Set starting sprite
		setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
	}

	void setPlayerStatus(PlayerEntity& pe) {
		pe.powerOfBombs += 1;
	}
};