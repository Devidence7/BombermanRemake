#include "../Include/EntitiesInclude.hpp"
#include "../Utils/IAFunctions.hpp"
#include "../Music/GameSounds.h"
#include "../GameMaths/GeometryMaths.hpp"
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
	setCollision(nullptr);
}

void EnemyEntity::generateMovements()
{
/*	std::vector<sf::Vector2i> objetives;
	for (Player_ptr player : PLayers::getVectorPlayer())
	{
		sf::Vector2f posPlayer = player->getCenterPosition();
		objetives.push_back(getMapCoordinates(posPlayer));
	}
	movements.clear();
	generatePath(this->me, objetives, movements);*/
	seekAnyPlayerOrRandom(movements, me, typeSeek);
	//movements = pathFinding(getMapCoordinates(this->getCenterPosition()), objetives, f);
	if(movements.size() < 1){
		generateRandomPath(getMapCoordinates(getCenterPosition()), movements, me);
	}
	numMovenet = numConsecutiveMovements;
	OmittedAreas.clear();
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
	if (onCollision)
	{
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
		if (movements.size() < 1 || numMovenet < 1)
		{
			this->generateMovements();
		}
		currentMovement = movements.front();
		movements.pop_front();
		numMovenet --;
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

	sf::Vector2i dir = currentMovement->getPosition();
	dir = dir - getEntityMapCoordinates();
	if(abs(dir.x) > abs(dir.y)){
		dir.y = 0;
	}else{
		dir.x = 0;
	}
	sf::Vector2f n= normalize(dir);
	velocity.x = baseSpeed *currentMovement->getAction().x * moveTime;
	velocity.y = baseSpeed *currentMovement->getAction().y * moveTime;
	//std::cout << "Vel: " << velocity.x << " " << velocity.y << std::endl;

	move(velocity.x, velocity.y);
}

void EnemyEntity::setExpiredEntity()
{
	if (GameTime::getTimeNow() - lastInvencibleTime < invencibleTime) {
		return;
	}

	spriteStartTime = GameTime::getTimeNow();
	spriteLastFrameTime = GameTime::getTimeNow();
	dyingEntity = true;
	GameSounds::playEnemyDied();
	//velocity = sf::Vector2f(0, 0);
}

void EnemyEntity::setCollision(std::shared_ptr<Entity> col){
	this->Entity::setCollision(col);
	if(col != nullptr){
		OmittedAreas.push_back(OmittedArea(getMapCoordinates(col->getCenterPosition())));
	}
}

void EnemyEntity::onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT)
{
	if (std::dynamic_pointer_cast<PlayerEntity>(eCollisioning) && !dyingEntity)
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
	return sf::FloatRect(dim.left, dim.top + 9, dim.width - 6, dim.height - 6);
}

void EnemyEntity::setInvencible1sec() {
	lastInvencibleTime = GameTime::getTimeNow();
}

Balloon::Balloon() : EnemyEntity()
{
	enemyType = balloon;
	scoreValue=100;
	//move(500, 100);
}

Ice::Ice() : EnemyEntity()
{
	enemyType = ice;
	scoreValue=150;
	this->rangoVision = 5;
	this->numConsecutiveMovements = 7;
	//this->typeSeek = TypeSeekIA::SECOND_BEST_PATH;
	this->typeSeek = TypeSeekIA::LONG_PATH;

	this->longMorePAth = 10;
	this->typePosIA = TypePositionRelative::OBJETIVE_POSITION;
	this->canThroughWall = false;
	//move(200, 200);
}

Barrel::Barrel() : EnemyEntity()
{
	enemyType = barrel;
	scoreValue=200;
	this->rangoVision = 7;
	this->numConsecutiveMovements = 5;
	this->typeSeek = TypeSeekIA::LONG_PATH;
	this->longMorePAth = 10;
	this->typePosIA = TypePositionRelative::LOKIN_POSITION;
	this->canThroughWall = false;
}
Coin::Coin() : EnemyEntity()
{
	enemyType = coin;
	scoreValue=400;
	baseSpeed = 1.5;
	this->rangoVision = 10;
	this->numConsecutiveMovements = 3;
	this->typeSeek = TypeSeekIA::BEST_PATH;
	this->longMorePAth = 10;
	this->typePosIA = TypePositionRelative::OBJETIVE_POSITION;
	this->canThroughWall = true;
}

Blob::Blob() : EnemyEntity()
{
	scoreValue=250;
	enemyType = blob;
	baseSpeed = 0.7;
	this->rangoVision = 5;
	this->numConsecutiveMovements = 4;
	this->typeSeek = TypeSeekIA::LONG_PATH;
	this->longMorePAth = 15;
	this->typePosIA = TypePositionRelative::NEAR_POSITION;
	this->canThroughWall = true;
}

Ghost::Ghost() : EnemyEntity()
{
	scoreValue=300;
	enemyType = ghost;
	this->rangoVision = 2;
	this->numConsecutiveMovements = 7;
	this->typeSeek = TypeSeekIA::LONG_PATH;
	this->longMorePAth = 20;
	this->typePosIA = TypePositionRelative::NEAR_POSITION;
	this->canThroughWall = true;
}

Hypo::Hypo() : EnemyEntity()
{
	scoreValue=350;
	enemyType = hypo;
	baseSpeed = 1.25;
	this->rangoVision = 8;
	this->numConsecutiveMovements = 5;
	this->typeSeek = TypeSeekIA::BEST_PATH;
	this->longMorePAth = 10;
	this->typePosIA = TypePositionRelative::OBJETIVE_POSITION;
	this->canThroughWall = true;
}
