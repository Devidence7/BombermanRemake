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

	Bomb(std::shared_ptr<PlayerEntity> p);

	void setExpiredEntity() override;
	bool isColliderWith(std::shared_ptr<Entity> eCollisioning) override;	
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;
	void update();

	void throwBomb(){
		onFlight = true;
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

	Fire(int type = 0);
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;

	void update() override;
};

