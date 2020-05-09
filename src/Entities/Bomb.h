#pragma once
#include <iostream>
#include "../Textures/BombTexture.h"
#include "../Textures/FireTexture.h"
#include "../Textures/TextureStorage.h"
#include "../Logic/Time.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

class Bomb : public Entity {
public:
	// Gameplay Variables:
	int bombPower;

	bool onMove = false;
	bool onFlight = false;

	bool rePutBomb = false;

	bool canExplote = true;

	// Texture Variables:
	BombTexture* bombTexture;
	double spriteCounter;
	const double spriteSpeed = 0.2;
	int actualFrame = 0;
	const int spriteFrames = 4;

	double explosionCounter;
	const double explosionTime = 3;

	double moveCounter;
	const double moveSpeed = 0.05;
	bool startRotation = true;
	float cornerModuleBefore = 999999;
	double lastMove;

	bool isRemoteBomb = false;
	bool signalExplote = false;
	// Player which bomb is from:
	std::shared_ptr<PlayerEntity> player;
	std::shared_ptr<PlayerEntity> player2Score;
	std::shared_ptr<Bomb> me;

	/***********
	 * Methods
	 **********/

	Bomb(std::shared_ptr<PlayerEntity> p);
	void setPosition(sf::Vector2f pos);
	sf::FloatRect getGlobalBounds() const override;
	void setExpiredEntity() override;
	bool isColliderWith(std::shared_ptr<Entity> eCollisioning) override;	
	void onCollission(std::shared_ptr<Entity> eCollisioning,std::shared_ptr<Entity> eCollisioner, CollisionType colT) override;
	void rotateBomb(double rotVel);
	sf::Vector2f getCornerPos();
	void update();
	void setPlayer2Score(std::shared_ptr<PlayerEntity> p){
		player2Score = p;
	}
	std::shared_ptr<PlayerEntity> getPlayer2Score(){
		return player2Score;
	} 
	void setOnFlight(sf::Vector2f dir, bool onf = true){
		onFlight = onf;
		this->velocity = sf::Vector2f(dir.x * this->baseSpeed, dir.y * this->baseSpeed);
		lastMove = GameTime::getTimeNow();
	}

	void setOnMove(sf::Vector2f dir, bool onm = true){
		onMove = onm;
		this->velocity = sf::Vector2f(dir.x * this->baseSpeed/2, dir.y * this->baseSpeed/2);
		lastMove = GameTime::getTimeNow();
	}
};


class Fire : public Entity {
public:
	FireTexture* fireTexture;
	std::shared_ptr<PlayerEntity> player2Score;

	double spriteStartTime;
	double spriteLastFrameTime;

	double spriteSpeed = 0.1;
	int currentFrame = 0;
	int spriteFrames = 5;

	double expiredTime = 0.5;
	int explosionType;

	Fire(std::shared_ptr<PlayerEntity> p, int type = 0);
	void onCollission(std::shared_ptr<Entity> eCollisioning,std::shared_ptr<Entity> eCollisioner, CollisionType colT) override;

	void update() override;
};

