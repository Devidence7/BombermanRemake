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
		this->collisioner = false;

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


	void update() {	}

	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override{
		std::cout<< "POWER UP\n";
		std::shared_ptr<PlayerEntity> pe;
		if(!this->expiredEntity && (pe = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr){
			this->setPlayerStatus(*pe);
			this->setExpiredEntity();
		}
	}

	virtual void setPlayerStatus(PlayerEntity &pe) {
		throw NoImplementedException("Intentando asignar clase virtual");
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

class LessFirePowerUp : public PowerUp {
public:
	LessFirePowerUp(sf::Vector2f pos) : PowerUp() {
		powerUpType = lessFire;

		// Set position
		setPosition(pos);

		// Set starting sprite
		setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
	}

	void setPlayerStatus(PlayerEntity& pe) {
		pe.powerOfBombs -= 1;
	}
};

class MoreSpeedPowerUp : public PowerUp {
public:
	MoreSpeedPowerUp(sf::Vector2f pos) : PowerUp() {
		powerUpType = moreSpeed;

		// Set position
		setPosition(pos);

		// Set starting sprite
		setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
	}

	void setPlayerStatus(PlayerEntity& pe) {
		pe.speedBoost += 0.2;
	}
};

class MoreBombsPowerUp : public PowerUp {
public:
	MoreBombsPowerUp(sf::Vector2f pos) : PowerUp() {
		powerUpType = moreBombs;

		// Set position
		setPosition(pos);

		// Set starting sprite
		setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
	}

	void setPlayerStatus(PlayerEntity& pe) {
		pe.numOfBombs += 1;
	}
};