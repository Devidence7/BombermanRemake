#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Textures/PlayerTexture.hpp"
#include "../Textures/TextureStorage.h"
#include "../Logic/Time.h"
#include <memory>
#include "../Logic/Time.h"


#include "Entity.h"
#include "Bomb.h"

using namespace std;

class PlayerEntity : public Entity
{
public:
	// Gameplay variables:
	int lives;
	double speedBoost = 1;
	int numOfBombs = 1;
	int powerOfBombs = 1;
	bool dead=false;
	sf::Sprite playerHead;
	sf::Sprite playerHead2;
	bool isBombKeyPresed = false;
	bool isActionKeyPresed = false;


	std::shared_ptr<PlayerEntity> me;

	unsigned int bombsTimeLimit = 30;

	// Texture varibles:
	double	animLastTic = 0;			// Last time frame changed
	int	currentFrame = 0;				// Frame we are now
	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 7;			// Number of death sprites
	const double frameSpeed = 0.15;		// Time between frames

	double lastMovementTime = 0;

	PlayerTexture* playerTexture;
	PlayerColor* playerColor;
	LookingAt lastMovement; // Save last looked direction

	Entity playerColorEntity;
	
	/*
	Constructor of Entity
	*/
	PlayerEntity();

	int getPowerOfBombs();

	void setExpiredEntity() override;

	Entity& getPlayerColorEntity();

	int getLives();

	inline void addLife(int numLives){
		this->lives += numLives;
	}

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity);

	sf::FloatRect getGlobalBounds() const override;

	Entity& playerUpdateColor();

	void setJumpingBomb() ;


	void update() override;
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;


	inline void takeBomb(std::shared_ptr<Entity> b){
		BombTaked = b;
	}

	inline void delBombTaked(){
		BombTaked.reset();
	}

	bool playerActions(int player);
	/*
	 * Update player position.
	 */
	bool updatePlayer(int ply);
	
};