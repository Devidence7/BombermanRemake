#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../Include/global.hpp"
#include "../Textures/EnemyTexture.h"
#include "../Utils/A_Star.hpp"
#include "Entity.h"

class EnemyEntity : public Entity {
private:
	EnemyTexture* enemyTexture;
	LookingAtBi lookingDir = bRight;
	double	animLastTic = 0;			// Actual tick
	int currentFrame = 0;				// Frame we are now

	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 5;

	const double frameSpeed = 0.15;		// Number of ticks between walking sprites

	bool dyingEntity = false;			// Starts entity dying animation


protected:
	EnemyType enemyType;
	
public:
	EnemyEntity() : Entity()
	{
		isFireDestroyable = true;
		fireCanGoThroght = true;
		onCollision = true;
		collisioner = false;

		updateVelocity();

		// Texture Controller
		enemyTexture = &TextureStorage::getEnemyTexture();
		// Set starting sprite
		setTextureRect(enemyTexture->getDefaultIntRect());
		// Set sprite Sheet texture
		setTexture(enemyTexture->getTexture());
	}

	void updateVelocity() {
		if(onCollision){
			int mul = Random::getIntNumberBetween(0, 1);
			if(!mul){
				mul = -1;
			}
			if(Random::getIntNumberBetween(0, 1)){
				velocity.y = baseSpeed*mul;
				velocity.x = 0;
			}else{
				velocity.y = 0;
				velocity.x = baseSpeed*mul;
			}
			onCollision = false;
		}
		//if (getGlobalBounds().top <= 0 && velocity.y < 0) {
		//	velocity.y = -velocity.y;
		//}
		//if (getGlobalBounds().top + enemyTexture->frameSize.y >= 600 && velocity.y > 0) {
		//	velocity.y = -velocity.y;
		//}
		//if (getGlobalBounds().left <= 0 && velocity.x < 0) {
		//	velocity.x = -velocity.x;
		//	lookingDir = bRight;
//
		//}
		//if (getGlobalBounds().left + enemyTexture->frameSize.x >= 800 && velocity.x > 0) {
		//	velocity.x = -velocity.x;
		//	lookingDir = bLeft;
		//}
		//// Move Entity position
		move(velocity.x, velocity.y);
	}

	void setExpiredEntity() override {
		dyingEntity = true;
	}
	
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override{
		if(std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)){
			eCollisioning->setExpiredEntity();
		}
	}

	void update() override
	{
		updateVelocity();

		// If the enemy has died:
		if (!dyingEntity) {
			// If there is speed we must update animation sprite every X time
			if (GameTime::getTimeNow() - animLastTic > frameSpeed) {
				setTextureRect(enemyTexture->getWalkingSprite(lookingDir, currentFrame, enemyType));
				currentFrame = (currentFrame + 1) % walkFrames;
				animLastTic = GameTime::getTimeNow();
			}
		}
		else {
			if (currentFrame == deathFrames - 1 && GameTime::getTimeNow() - animLastTic > frameSpeed) {
				expiredEntity = true;
			}

			else if (GameTime::getTimeNow() - animLastTic > frameSpeed) {
				setTextureRect(enemyTexture->getDeathSprite(currentFrame, enemyType));
				currentFrame = (currentFrame + 1) % deathFrames;
				animLastTic = GameTime::getTimeNow();
			}
		}
	}

	sf::FloatRect getGlobalBounds() const override
	{
		sf::FloatRect dim = sf::Sprite::getGlobalBounds();
		return sf::FloatRect(dim.left + 6, dim.top + 3, dim.width-6, dim.height-6);
	}
};


class Balloon : public EnemyEntity {
public:

	Balloon() :EnemyEntity() {
		enemyType = balloon;
		//move(500, 100);
	}
};

class Ice : public EnemyEntity {
public:

	Ice() :EnemyEntity() {
		enemyType = ice;
		//move(200, 200);
	}
};

class Barrel : public EnemyEntity {
public:

	Barrel() :EnemyEntity() {
		enemyType = barrel;
		//move(120, 500);
	}
};

class Coin : public EnemyEntity {
public:

	Coin() :EnemyEntity() {
		enemyType = coin;
		//move(500, 450);
	}
};

class Blob : public EnemyEntity {
public:

	Blob() :EnemyEntity() {
		enemyType = blob;
	//	move(400, 300);
	}
};

class Ghost : public EnemyEntity {
public:

	Ghost() :EnemyEntity() {
		enemyType = ghost;
	//	move(200, 400);
	}
};

class Hypo : public EnemyEntity {
public:

	Hypo() :EnemyEntity() {
		enemyType = hypo;
	//	move(400,600);
	}
};
