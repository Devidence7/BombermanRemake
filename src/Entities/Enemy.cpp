#include "../Include/EntitiesInclude.hpp"

EnemyEntity::EnemyEntity() : Entity()
{
	isFireDestroyable = true;
	fireCanGoThroght = true;
	canHurtPlayers = true;
	onCollision = false;
	collisioner = false;

	spriteLastFrameTime = GameTime::getTimeNow();

	// Texture Controller
	enemyTexture = &TextureStorage::getEnemyTexture();
	// Set starting sprite
	setTextureRect(enemyTexture->getDefaultIntRect());
	// Set sprite Sheet texture
	setTexture(enemyTexture->getTexture());
	colorPath = sf::Color(Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255));
}

void EnemyEntity::startMovement(){
	//generateRandomPath();
	/*generateRandomPath(getMapCoordinates(getCenterPosition()), movements);
	
	//this->generateMovements();
	currentMovement = movements.front();
	movements.pop_front();
	velocity.x = baseSpeed * currentMovement.getAction().x;
	velocity.y = baseSpeed * currentMovement.getAction().y;
	updateVelocity();*/
	setCollision();
}

void EnemyEntity::generateMovements()
{
	std::vector<sf::Vector2i> objetives;
	for (Player_ptr player : PLayers::getVectorPlayer())
	{
		sf::Vector2f posPlayer = player->getCenterPosition();
		objetives.push_back(getMapCoordinates(posPlayer));
	}
	bool f;
	movements.clear();
	generatePath(getMapCoordinates(this->getCenterPosition()), objetives, rangoVision, movements);
	//movements = pathFinding(getMapCoordinates(this->getCenterPosition()), objetives, f);
	if(movements.size() < 1){
		generateRandomPath(getMapCoordinates(getCenterPosition()), movements);
	}
}

void EnemyEntity::drawMovements(sf::RenderWindow &w)
{
	sf::FloatRect dim = this->getGlobalBounds();
	for (ANode_Ptr &an : movements)
	{
		sf::RectangleShape rece;
		rece.setSize(sf::Vector2f((dim.width / 2), (dim.height / 2)));
		rece.setFillColor(colorPath);
		sf::Vector2i posi = an->getPosition();
		sf::Vector2f posf = MapCoordinates2GlobalCoorCenter(an->getPosition());
		//std::cout << "pos " << posi.x << "( " << posf.x<< ")" << posi.y << "( " << posf.y<< ")" << std::endl;
		posf -= sf::Vector2f(dim.width / 4, dim.height / 4);
		rece.setPosition(posf);
		w.draw(rece);
	}
}


void EnemyEntity::updateVelocity()
{
	std::cout << "Update Velocity\n";
	if (onCollision)
	{
		//movements = generateRandomPath(getMapCoordinates(getCenterPosition()));
		this->generateMovements();
		if(movements.size() < 1){
			std::cout << "ERRORR movimientos vacios no esperados\n";
		}
		currentMovement = movements.front();
		movements.pop_front();
		//currentMovement.inverseDirection();
		onCollision = false;
	}
	else if (checkArrivePosition(this->getCenterPosition(), currentMovement->getPosition(), currentMovement->getAction()))
	{ //Si esta en posicions
		if (movements.size() < 1)
		{
			//movements = generateRandomPath(getMapCoordinates(getCenterPosition()));
			this->generateMovements();
		}
		currentMovement = movements.front();
		movements.pop_front();
	}

	double moveTime = 0;
	if (lastMovementTime) {
		moveTime = GameTime::getTimeNow() - lastMovementTime;
		moveTime *= 60;
	}
	lastMovementTime = GameTime::getTimeNow();

	if(currentMovement->getAction().x > 1 || currentMovement->getAction().y >1 ){
		std::cout<< "Error en accion\n";
		volatile int x = 0;
	}

	velocity.x = baseSpeed * currentMovement->getAction().x ;//* moveTime;
	velocity.y = baseSpeed * currentMovement->getAction().y ;//* moveTime;
	//std::cout << "Vel: " << velocity.x << " " << velocity.y << std::endl;

	move(velocity.x, velocity.y);
}

void EnemyEntity::setExpiredEntity()
{
	spriteStartTime = GameTime::getTimeNow();
	spriteLastFrameTime = GameTime::getTimeNow();
	dyingEntity = true;
	//velocity = sf::Vector2f(0, 0);
}

void EnemyEntity::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	if (std::dynamic_pointer_cast<PlayerEntity>(eCollisioning))
	{
		eCollisioning->setExpiredEntity();
	}
}

void EnemyEntity::update()
{
	// If the enemy has died:
	if (!dyingEntity)
	{
		updateVelocity();
		// If there is speed we must update animation sprite every X time
		if (GameTime::getTimeNow() - spriteLastFrameTime > spriteSpeed)
		{
			currentFrame = (currentFrame + 1) % spriteFrames;
			setTextureRect(enemyTexture->getWalkingSprite(lookingDir, currentFrame, enemyType));
			spriteLastFrameTime = GameTime::getTimeNow();
		}
	}
	else
	{
		if (GameTime::getTimeNow() - spriteStartTime > expiredTime) {
			expiredEntity = true;
		}
		else if (GameTime::getTimeNow() - spriteLastFrameTime > spriteSpeed) {
			spriteLastFrameTime = GameTime::getTimeNow();
			currentFrame = (currentFrame + 1) % deadSpriteFrames;
			setTextureRect(enemyTexture->getDeathSprite(currentFrame, enemyType));
		}
	}
}

sf::FloatRect EnemyEntity::getGlobalBounds() const
{
	sf::FloatRect dim = sf::Sprite::getGlobalBounds();
	return sf::FloatRect(dim.left + 6, dim.top + 3, dim.width - 6, dim.height - 6);
}

Balloon::Balloon() : EnemyEntity()
{
	enemyType = balloon;
	//move(500, 100);
}

Ice::Ice() : EnemyEntity()
{
	enemyType = ice;
	//move(200, 200);
}

Barrel::Barrel() : EnemyEntity()
{
	enemyType = barrel;
	//move(120, 500);
}
Coin::Coin() : EnemyEntity()
{
	enemyType = coin;
	//move(500, 450);
}

Blob::Blob() : EnemyEntity()
{
	enemyType = blob;
	//	move(400, 300);
}

Ghost::Ghost() : EnemyEntity()
{
	enemyType = ghost;
	//	move(200, 400);
}

Hypo::Hypo() : EnemyEntity()
{
	enemyType = hypo;
	//	move(400,600);
}
