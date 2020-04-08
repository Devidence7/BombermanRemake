#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Textures/PlayerTexture.hpp"
#include "../Textures/TextureStorage.h"
#include "../Logic/Time.h"
#include "Entity.h"

class PlayerEntity : public Entity
{
public:
	// Gameplay variables:
	int lifes;
	double speedBoost = 1;
	int numOfBombs = 1;
	int powerOfBombs = 1;

	unsigned int bombsTimeLimit = 30;

	// Texture varibles:
	double	animLastTic = 0;			// Last time frame changed
	int	currentFrame = 0;				// Frame we are now
	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 7;			// Number of death sprites
	const double frameSpeed = 0.15;		// Time between frames

	PlayerTexture *playerTexture;
	LookingAt lastMovement; // Save last looked direction
	
	/*
	Constructor of Entity
	*/
	PlayerEntity();

	int getPowerOfBombs();

	void setExpiredEntity() override;


	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity);
	sf::FloatRect getGlobalBounds() const override;

	void update() override;
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;



	/*
	 * Update player position.
	 */
	bool updatePlayer();
};