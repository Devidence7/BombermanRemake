#include "../Include/EntitiesInclude.hpp"
#include "../Music/GameSounds.h"

Bomb::Bomb(std::shared_ptr<PlayerEntity> p) : Entity()
{
	player = p;
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
	if (GameTime::getTimeNow() - explosionCounter > explosionTime)
	{
		setExpiredEntity();
	}

	if (GameTime::getTimeNow() - spriteCounter > spriteSpeed)
	{
		actualFrame = (actualFrame + 1) % spriteFrames;
		setTextureRect(bombTexture->getFrame(actualFrame));
		spriteCounter = GameTime::getTimeNow();
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
		actualFrame = (actualFrame + 1) % spriteFrames;
		setTextureRect(fireTexture->getFrame(actualFrame, explosionType));
	}
}
