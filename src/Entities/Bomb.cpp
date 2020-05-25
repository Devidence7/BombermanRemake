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

	shadow.setTexture(TextureStorage::getEntityShadowTexture().getTexture());
	shadow.setColor(sf::Color(255, 255, 255, 150));
}

void Bomb::drawEntity(sf::RenderWindow& window) {
	if (!onFlight) {
		window.draw(*this);
	}
	else {
		float flyingDistance = max(abs(endCenterFlight.x - initialCenterFlight.x), abs(endCenterFlight.y - initialCenterFlight.y));
		float flyingPoint = max(abs(initialCenterFlight.x - getCenterPosition().x), abs(initialCenterFlight.y - getCenterPosition().y));
		flyingPoint = (flyingPoint / flyingDistance) * 2.14 + 1;
		float upMaxValue = flyingDistance < 50 ? 30 : 100;

		float upValue = sin(flyingPoint) * upMaxValue;

		setPosition(sf::Vector2f(getPosition().x, getPosition().y - upValue));
		window.draw(*this);
		setPosition(sf::Vector2f(getPosition().x, getPosition().y + upValue));
	}
}

void Bomb::drawShadow(sf::RenderWindow& window) {
	shadow.setPosition(this->getPosition().x + 3, this->getPosition().y + 33);
	window.draw(shadow);
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
	/*cout << "-----------" << endl;
	cout << getOrigin().x << endl;
	cout << getPosition().x << endl;
	*/
	

	if (velocity.x < 0) {
		rotate(-100 * velMult);
	}
	else {
		rotate(100 * velMult);
	}
	/*cout << getOrigin().x << endl;
	cout << getPosition().x << endl;
*/
	
	// setOrigin(bombOri);
	// setPosition(bombPos);
}


sf::Vector2f Bomb::getPosition() {
	return sf::Vector2f(this->sf::Sprite::getPosition().x - 24, this->sf::Sprite::getPosition().y - 24);
}

void Bomb::setPosition(sf::Vector2f pos) {
	this->sf::Sprite::setPosition(sf::Vector2f(pos.x + 24, pos.y + 24));
}

sf::Vector2f Bomb::getCenterPosition() {
	return this->sf::Sprite::getPosition();
}

sf::Vector2f Bomb::getCenterPosition() const {
	return this->sf::Sprite::getPosition();
}

void Bomb::update()
{
	// If it is time to explote:
	if ((!isRemoteBomb && GameTime::getTimeNow() - explosionCounter > explosionTime || signalExplote) && !onFlight && canExplote )
	{
		setPosition(MapCoordinates2GlobalCoorCorner(getEntityMapCoordinates()));//Asegurarse de que se centra antes de esplotar
		setExpiredEntity();
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
		velMult *= 8;
		move(sf::Vector2f(velocity.x * velMult, velocity.y * velMult));
		double module = moduleVector(positionObjetive - getPosition());

		if (module > cornerModuleBefore || module < 1) {
			setPosition(positionObjetive);
			rePutBomb = true;
			onFlight = false;
			cornerModuleBefore = 99999;
		}
		else {
			cornerModuleBefore = module;
			rotateBomb(velMult);
		}
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
			rotateBomb(velMult);
		}
		lastMove = GameTime::getTimeNow();
	}

	
	
}
double Bomb::getExplosionTimeLeft() {
	return explosionTime - (GameTime::getTimeNow() - explosionCounter);
}

sf::FloatRect Bomb::getGlobalBounds() const
{
	sf::Vector2f pos = getCenterPosition();
	return sf::FloatRect(pos.x - 24, pos.y - 24, 48, 48);
}

Fire::Fire(Player_ptr p, int type ) : Entity()
{
	player2Score = p;
	spriteStartTime = GameTime::getTimeNow();
	spriteLastFrameTime = GameTime::getTimeNow();
	explosionType = type;

	fireCanGoThroght = true;
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
	if (!eCollisioning->getExpiredEntity()) {
		Enemy_ptr e;
		Player_ptr p;
		if ((e = std::dynamic_pointer_cast<EnemyEntity>(eCollisioning)) != nullptr) {
			if (!e->dyingEntity) {
				eCollisioning->setExpiredEntity();
				player2Score->incrementScore(eCollisioning->scoreValue);
			}
			
		}
		else if (std::dynamic_pointer_cast<Player_ptr>(eCollisioning) != nullptr) {
			if (!p->getExpiredEntity() && !p->dead && !p->respawning) {
				eCollisioning->setExpiredEntity();
				player2Score->incrementScore(eCollisioning->scoreValue);
			}
		}
		else {
			eCollisioning->setExpiredEntity();
			//player2Score->incrementScore(0);
		}
		
	}
	
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
