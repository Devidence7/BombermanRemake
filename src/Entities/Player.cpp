#include "../Include/EntitiesInclude.hpp"
#include "../Map/Level.hpp"

PlayerEntity::PlayerEntity() : Entity()
{
	isFireDestroyable = true;
	fireCanGoThroght = true;
	collisioner = false;

	animLastTic = GameTime::getTimeNow();
	baseSpeed = 2.5;
	lastMovement = LookingAt::down;
	lives=3;

	// Texture Controller
	playerTexture = &TextureStorage::getPlayerTexture();
	// Set starting sprite
	setTextureRect(playerTexture->getDefaultIntRect());
	// Set sprite Sheet texture
	setTexture(playerTexture->getTexture());

	// Texture Controller
	playerColor = &TextureStorage::getPlayerColor();
	// Set starting sprite
	playerColorEntity.setTextureRect(playerColor->getDefaultIntRect());
	// Set sprite Sheet texture
	playerColorEntity.setTexture(playerColor->getTexture());
	playerColorEntity.setColor(sf::Color(Random::getIntNumberBetween(0,255), Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255),225));

	// TODO: Remove this
	move(100, 100);
}

int PlayerEntity::getPowerOfBombs()
{
	return powerOfBombs;
}

void PlayerEntity::setExpiredEntity()
{
	if (!expiredEntity)
	{
		expiredEntity = true;
		currentFrame = 0;
		animLastTic = GameTime::getTimeNow();
	}
}

inline Entity& PlayerEntity::getPlayerColorEntity() {
	return playerColorEntity;
}


int PlayerEntity::getLives(){
	return lives;
}

/*
	Animate Entity by changing the actual sprite.
	*/
void PlayerEntity::animate(sf::Vector2f velocity)
{
	// If the player has died:
	if (!expiredEntity)
	{
	
		if (velocity.x == 0 && velocity.y == 0)
		{
			// If there is not speed set idle sprite
			setTextureRect(playerTexture->getIdleSprite(lastMovement));
			playerColorEntity.setTextureRect(playerTexture->getIdleSprite(lastMovement));
		}
		else
		{
			// If there is speed we must update animation sprite every X time
			if (GameTime::getTimeNow() - animLastTic > frameSpeed)
			{
				setTextureRect(playerTexture->getMoveSprite(lastMovement, currentFrame));
				playerColorEntity.setTextureRect(playerTexture->getMoveSprite(lastMovement, currentFrame));
				currentFrame = (currentFrame + 1) % walkFrames;
				animLastTic = GameTime::getTimeNow();
			}
		}
	}
	else
	{
		if (currentFrame == 6 && GameTime::getTimeNow() - animLastTic > frameSpeed)
		{
			if(lives>0){
		    	lives--;
			}
			//Else mostrar fin de partida
			expiredEntity = false;
			setPosition(100, 100);
		}

		if (GameTime::getTimeNow() - animLastTic > frameSpeed)
		{	
			setTextureRect(playerTexture->getDeathSprite(currentFrame));
			playerColorEntity.setTextureRect(playerTexture->getDeathSprite(currentFrame));
			currentFrame = (currentFrame + 1) % deathFrames;
			animLastTic = GameTime::getTimeNow();
		}
	}
}

sf::FloatRect PlayerEntity::getGlobalBounds() const
{
	sf::FloatRect dim = sf::Sprite::getGlobalBounds();
	return sf::FloatRect(dim.left + 15, dim.top + 45, 27, 27);
}

Entity& PlayerEntity::playerUpdateColor() {
	playerColorEntity.setPosition(getPosition());
	return playerColorEntity;
}

void PlayerEntity::update()
{
	return;
}

void PlayerEntity::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	return;
}

/**
	 * 
	 */

bool PlayerEntity::playerActions(){

	double t = GameTime::getTimeNow();
	bool playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	if (!playerBOMB){
		isBombKeyPresed = false;
	}else if(numOfBombs > 0 && !isBombKeyPresed){
		if(Level::addBomb(this->me)){
			numOfBombs--;
		}
		isBombKeyPresed = true;
	}
}


/*
	 * Update player position.
	 */
bool PlayerEntity::updatePlayer()
{
	// Player movement
	bool playerRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
	bool playerLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
	bool playerUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
	bool playerDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));

	bool playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	velocity.x = 0;
	velocity.y = 0;

	if (playerDown)
	{
		velocity.y = baseSpeed * speedBoost;
		lastMovement = LookingAt::down;
	}
	if (playerUp)
	{
		velocity.y = -baseSpeed * speedBoost;
		lastMovement = LookingAt::up;
	}
	if (playerLeft)
	{
		velocity.x = -baseSpeed * speedBoost;
		lastMovement = LookingAt::left;
	}
	if (playerRight)
	{
		velocity.x = baseSpeed * speedBoost;
		lastMovement = LookingAt::right;
		//lives--;
	}

	if (velocity.x != 0 && velocity.y != 0){
		float module = sqrt((velocity.x*velocity.x) + (velocity.y * velocity.y));
		velocity.x = (velocity.x/module) /* * sqrt(2)  */* baseSpeed;
		velocity.y = (velocity.y/module) /* * sqrt(2)  */* baseSpeed;
	}

	// Call animate function to change current sprite if needed.
	animate(velocity);

	// Move Entity position
	if (!expiredEntity)
	{
		move(velocity.x, velocity.y);
	}

	if (playerBOMB && numOfBombs > 0)
	{
		return true;
	}
	return false;
}


void PlayerEntity::setJumpingBomb() {
	//Actializar frames
	return;
}