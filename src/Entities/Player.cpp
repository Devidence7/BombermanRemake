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
	lives = 3;
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
	sf::Color tempColor = sf::Color(Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255), 225);
	playerColorEntity.setColor(tempColor);

	playerHead.setTexture(playerColor->getTexture());
	playerHead.setColor(tempColor);
	playerHead.setTextureRect(sf::IntRect(sf::Vector2i(10, 5), sf::Vector2i(36, 36)));

	playerHead2.setTexture(playerTexture->getTexture());
	playerHead2.setTextureRect(sf::IntRect(sf::Vector2i(10, 5), sf::Vector2i(36, 36)));

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

inline Entity &PlayerEntity::getPlayerColorEntity()
{
	return playerColorEntity;
}

int PlayerEntity::getLives()
{
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
			lives--;
			if (lives > 0)
			{

				expiredEntity = false;
				setPosition(100, 100);
			}
			else
			{
				//expiredEntity = true;
				setExpiredEntity();
			}
			/*else{
				gameDisplay.setGameState(GameDisplayController::GameState::GAME_OVER);
			}*/
			//Else mostrar fin de partida
			/*else{}
				expiredEntity = false;
			setPosition(100, 100);*/
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

Entity &PlayerEntity::playerUpdateColor()
{
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

void PlayerEntity::realizeActions()
{
	if (BombTaked != nullptr)
	{
		//lanzar
		Level::ThrowBomb(me, std::dynamic_pointer_cast<Bomb>(BombTaked));
		BombTaked.reset();
	}
	else
	{
		switch (this->actionAvaible)
		{
		case ActionsAvalible::GRAB_BOMB:
			Level::canTakeBomb(me);
			break;
		case ActionsAvalible::KICK_BOM:
			Level::canKickBomb(me);
			break;

		default:
			break;
		}
	}
}

/**
	 * 
	 */

bool PlayerEntity::playerActions(int player)
{

	double t = GameTime::getTimeNow();
	bool playerBOMB = false;
	bool actionButton = false;
	if (player == 1)
	{
		playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		actionButton = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	}
	else
	{
		playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
	}

	if (!playerBOMB)
	{
		isBombKeyPresed = false;
	}
	else if (numOfBombs > 0 && !isBombKeyPresed)
	{
		if (Level::addBomb(this->me))
		{
			numOfBombs--;
		}
		isBombKeyPresed = true;
	}

	if (!actionButton)
	{
		this->isActionKeyPresed = false;
	}
	else if (!isActionKeyPresed)
	{ //Si el boton accion y no se acaba de pulsar
		isActionKeyPresed = true;
		realizeActions();
	}

	return isBombKeyPresed;
}

/*
	 * Update player position.
	 */
bool PlayerEntity::updatePlayer(int ply)
{
	bool playerRight;
	bool playerLeft;
	bool playerUp;
	bool playerDown;
	bool playerBOMB;
	// Player movement

	/*bool playerRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));
	bool playerLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A));
	bool playerUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W));
	bool playerDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S));

	bool playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);*/
	if (ply == 1)
	{
		playerRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
		playerLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
		playerUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
		playerDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down));

		playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	}
	else
	{
		playerRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		playerLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		playerUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		playerDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

		playerBOMB = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
	}

	double moveTime = 0;
	if (lastMovementTime)
	{
		moveTime = GameTime::getTimeNow() - lastMovementTime;
	}
	lastMovementTime = GameTime::getTimeNow();

	velocity.x = 0;
	velocity.y = 0;

	if (playerDown)
	{
		velocity.y = baseSpeed * speedBoost * moveTime * 60;
		lastMovement = LookingAt::down;
	}
	if (playerUp)
	{
		velocity.y = -baseSpeed * speedBoost * moveTime * 60;
		lastMovement = LookingAt::up;
	}
	if (playerLeft)
	{
		velocity.x = -baseSpeed * speedBoost * moveTime * 60;
		lastMovement = LookingAt::left;
	}
	if (playerRight)
	{
		velocity.x = baseSpeed * speedBoost * moveTime * 60;
		lastMovement = LookingAt::right;
		//lives--;
	}

	// Call animate function to change current sprite if needed.
	animate(velocity);

	// Move Entity position
	if (!expiredEntity)
	{
		move(velocity.x, velocity.y);
		if (BombTaked != nullptr)
		{ //Si tiene bomba, actualizar a la posicion del jugador (centrado segun cuadricula)
			BombTaked->setPosition(Level::getMapCellCorner(this->getCenterPosition()));
		}
	}

	return false;
}
