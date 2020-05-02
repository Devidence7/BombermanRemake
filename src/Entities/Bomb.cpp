#include "../Include/EntitiesInclude.hpp"

Bomb::Bomb(std::shared_ptr<PlayerEntity> p) : Entity()
{
	player = p;
	p->numOfBombs -= 1;
	this->bombPower = p->getPowerOfBombs();
	isFireDestroyable = true;
	fireCanGoThroght = true;

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
	expiredEntity = true;
	player->numOfBombs += 1;
}

void Bomb::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	if (this->onFlight){
		return;
	}
	Player_ptr p;
	 if(!eCollisioning->CanThrowBomb()){
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
	spriteCounter = 0;
	spriteSpeed = 5;
	actualFrame = 0;
	spriteFrames = 5;
	explosionCounter = 0;
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
	explosionCounter++;
	if (explosionCounter >= 25)
	{
		expiredEntity = true;
	}

	spriteCounter++;
	spriteCounter %= spriteSpeed;
	if (spriteCounter == 0)
	{
		actualFrame = (actualFrame + 1) % spriteFrames;
		setTextureRect(fireTexture->getFrame(actualFrame, explosionType));
	}
}
