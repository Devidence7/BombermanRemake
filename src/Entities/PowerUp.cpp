#include "../Include/EntitiesInclude.hpp"


PowerUp::PowerUp() : Entity()
{
	isFireDestroyable = true;
	fireCanGoThroght = true;
	collisioner = false;
	scoreValue=50;
	spriteCounter = 0;
	spriteSpeed = 15;
	actualFrame = 0;
	spriteFrames = 3;
	explosionCounter = 0;
	creationTime = GameTime::getTimeNow();

	// Texture Controller:
	powerUpTexture = &TextureStorage::getPowerUpTexture();
	// Set sprite Sheet texture
	setTexture(powerUpTexture->getTexture());
}

void PowerUp::update() {
	if (GameTime::getTimeNow() - creationTime > powerUpLiveTime) {
		setExpiredEntity();
	}

	if (GameTime::getTimeNow() - creationTime > powerUpLiveTime * 3 / 4) {
		if (GameTime::getTimeNow() - lastTransparentTime > 0.2){
			if (transparent) {
				setColor(sf::Color(255, 255, 255, 255));
			}
			else {
				setColor(sf::Color(255, 255, 255, 100));
			}
			transparent = !transparent;
			lastTransparentTime = GameTime::getTimeNow();
		}
	}
}

void PowerUp::onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT)
{
	std::shared_ptr<PlayerEntity> pe;
	if (!this->expiredEntity && (pe = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr)
	{
		this->setPlayerStatus(*pe);
		pe->incrementScore(100);
		this->setExpiredEntity();
	}
}

void PowerUp::setPlayerStatus(PlayerEntity &pe)
{
	throw NoImplementedException("Intentando asignar clase abstracta (Power Up)");
}


MoreFirePowerUp::MoreFirePowerUp(sf::Vector2f pos) : BuffPoweUp()
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



LessFirePowerUp::LessFirePowerUp(sf::Vector2f pos) : NerfPoweUp()
{
	powerUpType = lessFire;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void LessFirePowerUp::setPlayerStatus(PlayerEntity& pe) {
	if (pe.powerOfBombs > 1) {
		pe.powerOfBombs -= 1;
	}
}


MoreSpeedPowerUp::MoreSpeedPowerUp(sf::Vector2f pos) : BuffPoweUp()
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



LessSpeedPowerUp::LessSpeedPowerUp(sf::Vector2f pos) : NerfPoweUp() {
	powerUpType = lessSpeed;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void LessSpeedPowerUp::setPlayerStatus(PlayerEntity& pe) {
	if (pe.speedBoost > 1.0) {
		pe.speedBoost -= 0.2;
	}
}



MoreBombsPowerUp::MoreBombsPowerUp(sf::Vector2f pos) : BuffPoweUp()
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


MoreTimePowerUp::MoreTimePowerUp(sf::Vector2f pos) : BuffPoweUp() {
	powerUpType = moreTime;
	
	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void MoreTimePowerUp::setPlayerStatus(PlayerEntity& pe) {
	cout<<"TIEMPO ANTES"<<GlobalTime::totalTime<<endl;
	GlobalTime::totalTime=GlobalTime::totalTime+30;
	cout<<"TIEMPO DESPUÉS"<<GlobalTime::totalTime<<endl;
}



GrabBombsPowerUp::GrabBombsPowerUp(sf::Vector2f pos) : ActionPoweUp() {
	powerUpType = grabBombs;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void GrabBombsPowerUp::setPlayerStatus(PlayerEntity& pe) {
	pe.setAction(ActionsAvalible::GRAB_BOMB);
}


KickBombsPowerUp::KickBombsPowerUp(sf::Vector2f pos) : ActionPoweUp() {
	powerUpType = kickBombs;
	
	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void KickBombsPowerUp::setPlayerStatus(PlayerEntity& pe) {
	pe.setAction(ActionsAvalible::KICK_BOM);
}


ExtraLifePowerUp::ExtraLifePowerUp(sf::Vector2f pos) : BuffPoweUp() {
	powerUpType = extraLife;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void ExtraLifePowerUp::setPlayerStatus(PlayerEntity& pe) {
	pe.addLife(1);
}


PassBombsPowerUp::PassBombsPowerUp(sf::Vector2f pos) : ActionPoweUp() {
	powerUpType = passBombs;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void PassBombsPowerUp::setPlayerStatus(PlayerEntity& pe) {
	pe.setAction(ActionsAvalible::THROUGH_BOMB);
}



DisseasePowerUp::DisseasePowerUp(sf::Vector2f pos) : NerfPoweUp() {
	powerUpType = dissease;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void DisseasePowerUp::setPlayerStatus(PlayerEntity& pe) {
	pe.invertControls();
}



RemoteBombPowerUp::RemoteBombPowerUp(sf::Vector2f pos) : ActionPoweUp() {
	powerUpType = remoteBombs;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void RemoteBombPowerUp::setPlayerStatus(PlayerEntity& pe) {
	pe.setAction(ActionsAvalible::REMOTE_BOMB);
}
