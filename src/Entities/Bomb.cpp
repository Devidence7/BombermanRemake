#include "../Include/EntitiesInclude.hpp"
#include "../Music/GameSounds.h"
#include "../Map/Level.hpp"

Bomb::Bomb(std::shared_ptr<PlayerEntity> p) : Entity() {
	player = p;
	player2Score = p;
	baseSpeed = 25; //TODO: HACER BIENdsds adee
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

	setOrigin(24,24);
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

void Bomb::rotateBomb(double velMult) {
	auto bombOri = getOrigin();
	auto bombPos = getPosition();
	auto bombLimits = getLocalBounds();
	cout << "-----------" << endl;
	cout << getOrigin().x << endl;
	cout << getPosition().x << endl;
	
	

	if (velocity.x < 0) {
		rotate(-100 * velMult);
	}
	else {
		rotate(100 * velMult);
	}
	cout << getOrigin().x << endl;
	cout << getPosition().x << endl;

	
	// setOrigin(bombOri);
	// setPosition(bombPos);
}


sf::Vector2f Bomb::getPosition() {
	return sf::Vector2f(this->sf::Sprite::getPosition().x - 24, this->sf::Sprite::getPosition().y - 24);
}

void Bomb::setPosition(sf::Vector2f pos) {
	this->sf::Sprite::setPosition(sf::Vector2f(pos.x + 24, pos.y + 24));
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
	}

	if (GameTime::getTimeNow() - moveCounter > moveSpeed) {
		moveCounter = GameTime::getTimeNow();
	}

	if (onFlight) {
		double velMult = GameTime::getTimeNow() - lastMove < 0.5 ? GameTime::getTimeNow() - lastMove : 0.5;
		velMult *= 10;
		move(sf::Vector2f(velocity.x * velMult, velocity.y * velMult));
		double module = moduleVector(positionObjetive - getPosition());

		if (module > cornerModuleBefore) {
			setPosition(positionObjetive);
			rePutBomb = true;
			onFlight = false;
			cornerModuleBefore = 99999;
			
			//move(-24, -24);
		}
		else {
			cornerModuleBefore = module;
		}
		rotateBomb(velMult);
		lastMove = GameTime::getTimeNow();
	}
	else if (onMove) {
		double velMult = GameTime::getTimeNow() - lastMove < 0.5 ? GameTime::getTimeNow() - lastMove : 0.5;
		velMult *= 20;
		if (onCollision) {
			onMove = false;
			onCollision = false;
		}
		else {
			move(sf::Vector2f(velocity.x * velMult, velocity.y * velMult));
		}
		// rotateBomb(velMult);
		lastMove = GameTime::getTimeNow();
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

	auto fireLimits = getLocalBounds();
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
