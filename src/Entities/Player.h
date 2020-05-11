#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Textures/PlayerTexture.hpp"
#include "../Textures/TextureStorage.h"
#include "../Logic/Time.h"
#include <memory>
#include "Entity.h"

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

	// Sprites for GUI
	sf::Sprite playerHead;
	sf::Sprite playerHead2;
	sf::Sprite playerCurrentAbility;

	bool isBombKeyPresed = false;
	bool isActionKeyPresed = false;

	sf::Vector2f initialPos;

	std::shared_ptr<PlayerEntity> me;

	unsigned int bombsTimeLimit = 30;

	// Texture varibles:
	double	animLastTic = 0;			// Last time frame changed
	int	currentFrame = 0;				// Frame we are now
	const int walkFrames = 4;			// Number of walking sprites
	const int deathFrames = 8;			// Number of death sprites
	const double frameSpeed = 0.15;		// Time between frames

	double lastMovementTime = 0;

	bool controlsInverted = false;
	double lastControlInvertedTime = 0;

	bool respawning = false;
	const double respawnTime = 5;
	double lastRespawnTime = 0;
	double lastInvencibleTime = 0;
	const double invencibleTime = 5;

	bool bombThrowed = false;
	double lastThrowedTime = 0;

	PlayerTexture* playerTexture;
	PlayerColor* playerColor;
	LookingAt lastMovement;				// Save last looked direction
	Entity playerColorEntity;

	struct PlayerControls {
		sf::Keyboard::Key goUp;
		sf::Keyboard::Key goDown;
		sf::Keyboard::Key goRight;
		sf::Keyboard::Key goLeft;
		sf::Keyboard::Key UseBomb;
		sf::Keyboard::Key MakeAction;
	};
	PlayerControls &playerControls;

	/*
	Constructor of Entity
	*/
	PlayerEntity(PlayerControls& playerControls, int _team,float posX,float posY);

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
	void animate(sf::Vector2f velocity,int posX,int posY);

	sf::FloatRect getGlobalBounds() const override;

	Entity& playerUpdateColor();

	void setCanThroughBomb(bool _ctb) override{
		return; //No se puede asignar la posibilidad de atravesar bombas sin action
	}

	void update() override;
	void onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT) override;


	inline void takeBomb(std::shared_ptr<Entity> b){
		BombTaked = b;
	}

	inline void delBombTaked(){
		BombTaked.reset();
	}

	void realizeActions();

	virtual bool playerActions();
	/*
	 * Update player position.
	 */
	virtual bool updatePlayer(int posX,int posY);

	void setJumpingBomb();

	void invertControls();

	void bombExploted(std::shared_ptr<Entity> b);

	inline bool haveBombs(){
		return this->numOfBombs > 0;
	}

	std::shared_ptr<Entity> getBomb();

	//void changeBombs2time();

};
