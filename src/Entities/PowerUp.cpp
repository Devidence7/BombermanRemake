#include "../Include/EntitiesInclude.hpp"


PowerUp::PowerUp() : Entity()
{
	isFireDestroyable = true;
	fireCanGoThroght = true;
	this->collisioner = false;

	spriteCounter = 0;
	spriteSpeed = 15;
	actualFrame = 0;
	spriteFrames = 3;
	explosionCounter = 0;

	// Texture Controller:
	powerUpTexture = &TextureStorage::getPowerUpTexture();
	// Set sprite Sheet texture
	setTexture(powerUpTexture->getTexture());
}

void PowerUp::update() {}

void PowerUp::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	std::cout << "POWER UP\n";
	std::shared_ptr<PlayerEntity> pe;
	if (!this->expiredEntity && (pe = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr)
	{
		this->setPlayerStatus(*pe);
		this->setExpiredEntity();
	}
}

void PowerUp::setPlayerStatus(PlayerEntity &pe)
{
	throw NoImplementedException("Intentando asignar clase ");
}

MoreFirePowerUp::MoreFirePowerUp(sf::Vector2f pos) : PowerUp()
{
	powerUpType = moreFire;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void MoreFirePowerUp::setPlayerStatus(PlayerEntity &pe)
{
	pe.powerOfBombs += 1;
}

LessFirePowerUp::LessFirePowerUp(sf::Vector2f pos) : PowerUp()
{
	powerUpType = lessFire;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void LessFirePowerUp::setPlayerStatus(PlayerEntity &pe)
{
	pe.powerOfBombs -= 1;
}
MoreSpeedPowerUp::MoreSpeedPowerUp(sf::Vector2f pos) : PowerUp()
{
	powerUpType = moreSpeed;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void MoreSpeedPowerUp::setPlayerStatus(PlayerEntity &pe)
{
	pe.speedBoost += 0.2;
}

MoreBombsPowerUp::MoreBombsPowerUp(sf::Vector2f pos) : PowerUp()
{
	powerUpType = moreBombs;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void MoreBombsPowerUp::setPlayerStatus(PlayerEntity &pe)
{
	pe.numOfBombs += 1;
}
