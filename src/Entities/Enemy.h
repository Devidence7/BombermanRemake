#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "../Textures/EnemyTexture.h"
#include "../Include/global.hpp"
#include "../Utils/A_Star.hpp"

class EnemyEntity : public Entity {
private:
	EnemyTexture* enemyTexture;
	LookingAtBi lookingDir = bRight;

	double spriteStartTime;
	double spriteLastFrameTime;

	double spriteSpeed = 0.2;			// Time between walking frames
	int currentFrame = 0;				// Current frame number
	int spriteFrames = 4;				// Number of walking frames

	double expiredspriteSpeed = 0.4;	// Time between dead frames
	double expiredTime = 1;				// Time for delete entity
	int deadSpriteFrames = 3;

	bool dyingEntity = false;			// Starts entity dying animation

	double lastMovementTime = 0;

	ANode currentMovement;

	sf::Color colorPath;

protected:
	std::list<ANode> movements;
	EnemyType enemyType;
	
public:
	EnemyEntity();

	void startMovement();

	ANode generateRandomMovement(sf::Vector2i fromPosition);

	void generateRandomPath();

	virtual void generateMovements();

	void updateVelocity();

	void setExpiredEntity() override;

	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;

	void update() override;

	sf::FloatRect getGlobalBounds() const override;

	void drawMovements(sf::RenderWindow &w);

};


class Balloon : public EnemyEntity {
public:

	Balloon();
};

class Ice : public EnemyEntity {
public:

	Ice();
};

class Barrel : public EnemyEntity {
public:

	Barrel();
};

class Coin : public EnemyEntity {
public:

	Coin();
};

class Blob : public EnemyEntity {
public:

	Blob();
};

class Ghost : public EnemyEntity {
public:

	Ghost();
};

class Hypo : public EnemyEntity {
public:

	Hypo();
};
