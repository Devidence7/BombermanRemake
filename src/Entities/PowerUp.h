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

	void onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT) override;

	virtual void setPlayerStatus(PlayerEntity &pe);
};


class ActionPoweUp : public PowerUp{
	public:
	virtual void setPlayerStatus(PlayerEntity& pe) = 0;
};

class BuffPoweUp : public PowerUp{
	public:
	virtual void setPlayerStatus(PlayerEntity& pe) = 0;
};

class NerfPoweUp : public PowerUp{
	public:
	virtual void setPlayerStatus(PlayerEntity& pe) = 0;
};


class MoreFirePowerUp : public BuffPoweUp {
public:
	MoreFirePowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class LessFirePowerUp : public NerfPoweUp {
public:
	LessFirePowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class MoreSpeedPowerUp : public BuffPoweUp {
public:
	MoreSpeedPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class LessSpeedPowerUp : public NerfPoweUp {
public:
	LessSpeedPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class MoreBombsPowerUp : public BuffPoweUp {
public:
	MoreBombsPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};



class MoreTimePowerUp : public BuffPoweUp {
public:
	MoreTimePowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class GrabBombsPowerUp : public ActionPoweUp {
public:
	GrabBombsPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class KickBombsPowerUp : public ActionPoweUp {
public:
	KickBombsPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

class ExtraLifePowerUp : public BuffPoweUp {
public:
	ExtraLifePowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
	
};

// Dizzyness
class DisseasePowerUp : public NerfPoweUp {
public:
	DisseasePowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
};

class PassBombsPowerUp : public ActionPoweUp {
public:
	PassBombsPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
};

class RemoteBombPowerUp : public ActionPoweUp {
public:
	RemoteBombPowerUp(sf::Vector2f pos);
	void setPlayerStatus(PlayerEntity& pe);
};

