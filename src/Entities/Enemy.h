#pragma once
#include "../Include/global.hpp"
#include "../Textures/EnemyTexture.h"
#include "Entity.h"

class EnemyEntity : public Entity {
private:
	EnemyTexture* enemyTexture;
	LookingAtBi lookingDir = bRight;
	int	animationCounter = 0;	// Actual tick
	int	currentFrame = 0;		// Frame we are now
	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 5;
	const int	framesSpeed = 8;		// Number of ticks between walking sprites

protected:
	EnemyType enemyType;
	
public:
	EnemyEntity(EnemyTexture& et) : Entity(et), enemyTexture(&et)
	{
		velocity.x = baseSpeed;
		velocity.y = baseSpeed;
	}

	void updateVelocity() {
		if (getGlobalBounds().top <= 0 && velocity.y < 0) {
			velocity.y = -velocity.y;
		}
		if (getGlobalBounds().top + enemyTexture->frameSize.y >= 600 && velocity.y > 0) {
			velocity.y = -velocity.y;
		}
		if (getGlobalBounds().left <= 0 && velocity.x < 0) {
			velocity.x = -velocity.x;
			lookingDir = bRight;

		}
		if (getGlobalBounds().left + enemyTexture->frameSize.x >= 800 && velocity.x > 0) {
			velocity.x = -velocity.x;
			lookingDir = bLeft;
		}
		// Move Entity position
		move(velocity.x, velocity.y);
	}

	void update() override
	{
		updateVelocity();
		
		// Call animate function to change current sprite if needed.
		animationCounter++;
		animationCounter %= framesSpeed;
		if (!expiredEntity) {
			if (animationCounter == 0) {
				currentFrame = (currentFrame + 1) % walkFrames;
				setTextureRect(enemyTexture->getWalkingSprite(lookingDir, currentFrame, enemyType));
			}
		}
		else {
			if (animationCounter == 0) {
				currentFrame = (currentFrame + 1) % deathFrames;
				setTextureRect(enemyTexture->getDeathSprite(currentFrame, enemyType));
			}
		}
	}
};


class Balloon : public EnemyEntity {
public:

	Balloon(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = balloon;
		move(500, 100);
	}
};

class Ice : public EnemyEntity {
public:

	Ice(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = ice;
		move(200, 200);
	}
};

class Barrel : public EnemyEntity {
public:

	Barrel(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = barrel;
		move(120, 500);
	}
};

class Coin : public EnemyEntity {
public:

	Coin(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = coin;
		move(500, 450);
	}
};

class Blob : public EnemyEntity {
public:

	Blob(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = blob;
		move(400, 300);
	}
};

class Ghost : public EnemyEntity {
public:

	Ghost(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = ghost;
		move(200, 400);
	}
};

class Hypo : public EnemyEntity {
public:

	Hypo(EnemyTexture& et) :EnemyEntity(et) {
		enemyType = hypo;
		move(400,600);
	}
};
