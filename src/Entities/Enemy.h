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
	double	animLastTic = 0;			// Actual tick
	int currentFrame = 0;				// Frame we are now

	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 5;

	const double frameSpeed = 0.15;		// Number of ticks between walking sprites

	bool dyingEntity = false;			// Starts entity dying animation

	sf::Color colorPath;

protected:
	std::vector<ANode> movements;
	EnemyType enemyType;
	
public:
	EnemyEntity();
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
