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
	sf::Sprite shadow;

	double spriteStartTime;
	double spriteLastFrameTime;

	double spriteSpeed = 0.2;			// Time between walking frames
	int currentFrame = 0;				// Current frame number
	int spriteFrames = 4;				// Number of walking frames

	double expiredspriteSpeed = 0.2;	// Time between dead frames
	double expiredTime = 1;				// Time for delete entity
	int deadSpriteFrames = 5;

	bool dyingEntity = false;			// Starts entity dying animation

	double lastMovementTime = 0;

	double lastInvencibleTime = 0;
	const double invencibleTime = 1;

	

	protected:


	ANode_Ptr currentMovement;

	sf::Color colorPath;

protected:
	std::list<ANode_Ptr> movements;
	EnemyType enemyType;
	
public:
	std::shared_ptr<EnemyEntity> me;
	EnemyEntity(float difficultyLevel);

	void drawShadow(sf::RenderWindow& window) override;

	void startMovement();

	virtual void generateMovements();

	void updateVelocity();

	void setExpiredEntity() override;

	void onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT) override;

	void setCollision(std::shared_ptr<Entity> col) override;

	void update() override;

	sf::FloatRect getGlobalBounds() const override;

	void setInvencible1sec();

	

	void drawMovements(sf::RenderWindow &w);



};


class Balloon : public EnemyEntity {
public:

	Balloon(float difficultyLevel);
};

//O'neal
class Ice : public EnemyEntity {
public:

	Ice(float difficultyLevel);
};

//Dahl
class Barrel : public EnemyEntity {
public:

	Barrel(float difficultyLevel);
};

//Pontan
class Coin : public EnemyEntity {
public:

	Coin(float difficultyLevel);
};

//Doria
class Blob : public EnemyEntity {
public:

	Blob(float difficultyLevel);
};

//Ovape
class Ghost : public EnemyEntity {
public:

	Ghost(float difficultyLevel);
};

//Pass
class Hypo : public EnemyEntity {
public:

	Hypo(float difficultyLevel);
};

