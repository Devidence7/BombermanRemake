#pragma once
#include "Entity.h"
#include "Player.h"
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
	PowerUp();

	void update();

	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;

	virtual void setPlayerStatus(PlayerEntity &pe);

	//void PowerUp::addPowerUp2Player(PlayerEntity &pe);

};

class MoreFirePowerUp : public PowerUp {
public:
	MoreFirePowerUp(sf::Vector2f pos);

	void setPlayerStatus(PlayerEntity& pe);
};

class LessFirePowerUp : public PowerUp {
public:
	LessFirePowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
};

class MoreSpeedPowerUp : public PowerUp {
public:
	MoreSpeedPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
};

class MoreBombsPowerUp : public PowerUp {
public:
	MoreBombsPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
};