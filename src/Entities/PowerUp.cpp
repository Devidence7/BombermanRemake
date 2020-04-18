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
	std::shared_ptr<PlayerEntity> pe;
	if (!this->expiredEntity && (pe = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr)
	{
		this->setPlayerStatus(*pe);
		this->setExpiredEntity();
		this->addPowerUp2Player(*pe);
	}
}





void PowerUp::setPlayerStatus(PlayerEntity &pe)
{
	throw NoImplementedException("Intentando asignar clase ");
}

void PowerUp::addPowerUp2Player(PlayerEntity &pe){
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

void MoreFirePowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	//sf::IntRect pu=getTextureRect();
	//pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
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
	if (pe.powerOfBombs > 1) {
		pe.powerOfBombs -= 1;
	}
}

void LessFirePowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = lessFire;
	//sf::IntRect pu=getTextureRect();
	//pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
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

void MoreSpeedPowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
}


LessSpeedPowerUp::LessSpeedPowerUp(sf::Vector2f pos) : PowerUp() {
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

void LessSpeedPowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
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

void MoreBombsPowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
}

MoreTimePowerUp::MoreTimePowerUp(sf::Vector2f pos) : PowerUp() {
	powerUpType = moreTime;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void MoreTimePowerUp::setPlayerStatus(PlayerEntity& pe) {
	
}

void MoreTimePowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
}

GrabBombsPowerUp::GrabBombsPowerUp(sf::Vector2f pos) : PowerUp() {
	powerUpType = grabBombs;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void GrabBombsPowerUp::setPlayerStatus(PlayerEntity& pe) {
	
}

void GrabBombsPowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
}

KickBombsPowerUp::KickBombsPowerUp(sf::Vector2f pos) : PowerUp() {
	powerUpType = kickBombs;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void KickBombsPowerUp::setPlayerStatus(PlayerEntity& pe) {
	
}

void KickBombsPowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
}


ExtraLifePowerUp::ExtraLifePowerUp(sf::Vector2f pos) : PowerUp() {
	powerUpType = extraLife;

	// Set position
	setPosition(pos);

	// Set starting sprite
	setTextureRect(powerUpTexture->getFrame(powerUpType, 0));
}

void ExtraLifePowerUp::setPlayerStatus(PlayerEntity& pe) {
	
}

void ExtraLifePowerUp::addPowerUp2Player(PlayerEntity &pe){
	powerUpType = moreFire;
	sf::IntRect pu=getTextureRect();
	pe.activatedPowerUps.push_back(pu);
	//pe.activatedPowerUps.push_back(setTextureRect(powerUpTexture->getFrame(powerUpType, 0)));
}
