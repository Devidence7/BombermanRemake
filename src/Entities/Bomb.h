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

	// Texture Variables:
	BombTexture* bombTexture;
	double spriteCounter;
	const double spriteSpeed = 0.2;
	int actualFrame = 0;
	const int spriteFrames = 4;

	double explosionCounter;
	const double explosionTime = 1.35;

	// Player which bomb is from:
	std::shared_ptr<PlayerEntity> player;

	/***********
	 * Methods
	 **********/

	Bomb(std::shared_ptr<PlayerEntity> p);
	sf::FloatRect getGlobalBounds() const override;
	void setExpiredEntity() override;
	bool isColliderWith(std::shared_ptr<Entity> eCollisioning) override;	
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;
	void update();
	void setOnFlight(sf::Vector2f dir, bool onf = true){
		onFlight = onf;
		this->velocity = sf::Vector2f(dir.x * this->baseSpeed, dir.y * this->baseSpeed);
	}

	void setOnMove(sf::Vector2f dir, bool onm = true){
		onMove = onm;
		this->velocity = sf::Vector2f(dir.x * this->baseSpeed, dir.y * this->baseSpeed);
		std::cout << "v: " << velocity.x << " " << velocity.y << "\n";
	}


};


class Fire : public Entity {
public:
	FireTexture* fireTexture;

	double spriteStartTime;
	double spriteLastFrameTime;

	double spriteSpeed = 0.1;
	int currentFrame = 0;
	int spriteFrames = 5;

	double expiredTime = 0.5;
	int explosionType;

	Fire(int type = 0);
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;

	void update() override;
};

