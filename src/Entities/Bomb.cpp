#include "../Include/EntitiesInclude.hpp"
#include "../Music/GameSounds.h"
#include "../Map/Level.hpp"

Bomb::Bomb(std::shared_ptr<PlayerEntity> p) : Entity()
{
	player = p;
	baseSpeed = 25; //TODO: HACER BIEN
	this->bombPower = p->getPowerOfBombs();
	isFireDestroyable = true;
	fireCanGoThroght = false;

	explosionCounter = GameTime::getTimeNow();
	spriteCounter = GameTime::getTimeNow();

	// Texture Controller:
	bombTexture = &TextureStorage::getBombTexture();
	// Set starting sprite
	setTextureRect(bombTexture->getFrame(0));
	// Set sprite Sheet texture
	setTexture(bombTexture->getTexture());
}

void Bomb::setExpiredEntity()
{
	GameSounds::playBombSound();
	expiredEntity = true;
	player->numOfBombs += 1;
}

bool Bomb::isColliderWith(Entity_ptr eCollisioning){
	return !eCollisioning->CanThroughBomb();
}	


void Bomb::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	if (this->onFlight){
		return;
	}
	Player_ptr p;
	 if(!eCollisioning->CanThroughBomb()){
		Entity::onCollission(eCollisioning, colT);
	}
	else  if ( (p = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr)
	{
		p->setJumpingBomb();
	}
	else if (std::dynamic_pointer_cast<EnemyEntity>(eCollisioning) != nullptr)
	{
		Entity::onCollission(eCollisioning, colT);
	}
	else
	{
		//stop Bomb? Fire?
		Entity::onCollission(eCollisioning, colT);
	}
}



void Bomb::update()
{
	// If it is time to explote:
	if (GameTime::getTimeNow() - explosionCounter > explosionTime && !onFlight)
	{
		setExpiredEntity();
		
	}

	if (GameTime::getTimeNow() - spriteCounter > spriteSpeed)
	{
		actualFrame = (actualFrame + 1) % spriteFrames;
		setTextureRect(bombTexture->getFrame(actualFrame));
		spriteCounter = GameTime::getTimeNow();
		if(onFlight){
			sf::Vector2f ff = this->getCenterPosition();
			//std::cout <<(ff - positionObjetive).x << " " << (ff - positionObjetive).y << "\n";
			//std::cout << "distancia: " << moduleVector(positionObjetive - this->getPosition()) << "\n";
			move(velocity);
			if (moduleVector(positionObjetive - this->getPosition()) < baseSpeed) {
				this->setPosition(positionObjetive);
				rePutBomb = true;
				onFlight = false;
			}
		}
	}
}

Fire::Fire(int type ) : Entity()
{
	spriteStartTime = GameTime::getTimeNow();
	spriteLastFrameTime = GameTime::getTimeNow();
	explosionType = type;

	// Texture Controller:
	fireTexture = &TextureStorage::getFireTexture();
	// Set starting sprite
	setTextureRect(fireTexture->getFrame(0, type));
	// Set sprite Sheet texture
	setTexture(fireTexture->getTexture());
}

void Fire::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	eCollisioning->setExpiredEntity();
}

void Fire::update()
{
	if (GameTime::getTimeNow() - spriteStartTime > expiredTime)
	{
		setExpiredEntity();
	}
	else if (GameTime::getTimeNow() - spriteLastFrameTime > spriteSpeed)
	{
		spriteLastFrameTime = GameTime::getTimeNow();
		currentFrame = (currentFrame + 1) % spriteFrames;
		setTextureRect(fireTexture->getFrame(currentFrame, explosionType));
	}
}
