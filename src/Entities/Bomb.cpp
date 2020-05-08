#include "../Include/EntitiesInclude.hpp"
#include "../Music/GameSounds.h"
#include "../Map/Level.hpp"

Bomb::Bomb(std::shared_ptr<PlayerEntity> p) : Entity()
{
	player = p;
	player2Score = p;
	baseSpeed = 25; //TODO: HACER BIEN
	this->bombPower = p->getPowerOfBombs();
	isFireDestroyable = true;
	fireCanGoThroght = false;
	isRemoteBomb = p->getActionAssigned() == ActionsAvalible::REMOTE_BOMB;

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
	player->bombExploted(this->me);
}

bool Bomb::isColliderWith(Entity_ptr eCollisioning){
	return !eCollisioning->CanThroughBomb();
}	


void Bomb::onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT)
{
	if (this->onFlight){
		return;
	}
	Player_ptr p;
	 if(!eCollisioning->CanThroughBomb()){
		Entity::onCollission(eCollisioning, eCollisioner,colT);
	}
	else  if ( (p = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr)
	{
		p->setJumpingBomb();
	}
	else if (std::dynamic_pointer_cast<EnemyEntity>(eCollisioning) != nullptr)
	{
		Entity::onCollission(eCollisioning, eCollisioner, colT);
	}
	else
	{
		//stop Bomb? Fire?
		Entity::onCollission(eCollisioning, eCollisioner, colT);
	}
}



void Bomb::update()
{
	// If it is time to explote:
	if ((!isRemoteBomb && GameTime::getTimeNow() - explosionCounter > explosionTime || signalExplote) && !onFlight && canExplote )
	{
		setExpiredEntity();
		setPosition(Level::getMapCellCorner(getCenterPosition()));//Asegurarse de que se centra antes de esplotar
		
	}

	if (GameTime::getTimeNow() - spriteCounter > spriteSpeed)
	{
		actualFrame = (actualFrame + 1) % spriteFrames;
		setTextureRect(bombTexture->getFrame(actualFrame));
		spriteCounter = GameTime::getTimeNow();
		if(onFlight){
			move(velocity);
			if (moduleVector(positionObjetive - this->getPosition()) < baseSpeed) {
				this->setPosition(positionObjetive);
				rePutBomb = true;
				onFlight = false;
			}
		}else if(onMove)
		{
			if(onCollision){
				onMove = false;
				onCollision = false;
			}else{
				move(velocity);
			}
		}
		
	}
}
sf::FloatRect Bomb::getGlobalBounds() const
{
	sf::FloatRect dim = sf::Sprite::getGlobalBounds();
	return sf::FloatRect(dim.left + 3, dim.top + 3, dim.width - 6, dim.height - 6);
}

Fire::Fire(Player_ptr p, int type ) : Entity()
{
	player2Score = p;
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

void Fire::onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT)
{
	eCollisioning->setExpiredEntity();
	//TODO: Check if have increment score
	player2Score->incrementScore(0);
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
