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
	int lives;
	double speedBoost = 1;
	int numOfBombs = 1;
	int powerOfBombs = 1;
	//list<PowerUpTexture*>activatedPowerUps;

	unsigned int bombsTimeLimit = 30;

	// Texture varibles:
	double	animLastTic = 0;			// Last time frame changed
	int	currentFrame = 0;				// Frame we are now
	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 7;			// Number of death sprites
	const double frameSpeed = 0.15;		// Time between frames

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

	/*
	Animate Entity by changing the actual sprite.
	*/
	void animate(sf::Vector2f velocity);

	sf::FloatRect getGlobalBounds() const override;

	Entity& playerUpdateColor();

	void update() override;
	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;



	/*
	 * Update player position.
	 */
	bool updatePlayer();
};