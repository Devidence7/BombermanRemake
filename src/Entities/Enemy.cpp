#include "../Include/EntitiesInclude.hpp"

EnemyEntity::EnemyEntity() : Entity()
{
	isFireDestroyable = true;
	fireCanGoThroght = true;
	canHurtPlayers = true;
	onCollision = true;
	collisioner = false;

	updateVelocity();
	// Texture Controller
	enemyTexture = &TextureStorage::getEnemyTexture();
	// Set starting sprite
	setTextureRect(enemyTexture->getDefaultIntRect());
	// Set sprite Sheet texture
	setTexture(enemyTexture->getTexture());

}

void EnemyEntity::generateMovements()
{
	std::vector<sf::Vector2i> objetives;
	for (Player_ptr player : PLayers::getVectorPlayer())
	{
		sf::Vector2f posPlayer = player->getCenterPosition();
		objetives.push_back(getMapCoordinates(posPlayer));
	}

	movements = pathFinding(getMapCoordinates(this->getCenterPosition()), objetives);

}

void EnemyEntity::drawMovements(sf::RenderWindow &w){

	sf::FloatRect dim = this->getGlobalBounds();
	for(ANode &an : movements){
		sf::RectangleShape rece;
		rece.setSize(sf::Vector2f((dim.width/2), (dim.height/2)));
		rece.setFillColor(sf::Color(255, 255, 0));
		sf::Vector2i posi = an.getPosition();
		sf::Vector2f posf = MapCoordinates2GlobalCoorCenter(an.getPosition());
		//std::cout << "pos " << posi.x << "( " << posf.x<< ")" << posi.y << "( " << posf.y<< ")" << std::endl;
		posf -= sf::Vector2f(dim.width/4, dim.height/4);
		rece.setPosition(posf);
		w.draw(rece);
	}
}

void EnemyEntity::updateVelocity()
{
	if (onCollision)
	{
		int mul = Random::getIntNumberBetween(0, 1);
		if (!mul)
		{
			mul = -1;
		}
		if (Random::getIntNumberBetween(0, 1))
		{
			velocity.y = baseSpeed * mul;
			velocity.x = 0;
		}
		else
		{
			velocity.y = 0;
			velocity.x = baseSpeed * mul;
		}
		onCollision = false;
	}
	//if (getGlobalBounds().top <= 0 && velocity.y < 0) {
	//	velocity.y = -velocity.y;
	//}
	//if (getGlobalBounds().top + enemyTexture->frameSize.y >= 600 && velocity.y > 0) {
	//	velocity.y = -velocity.y;
	//}
	//if (getGlobalBounds().left <= 0 && velocity.x < 0) {
	//	velocity.x = -velocity.x;
	//	lookingDir = bRight;
	//
	//}
	//if (getGlobalBounds().left + enemyTexture->frameSize.x >= 800 && velocity.x > 0) {
	//	velocity.x = -velocity.x;
	//	lookingDir = bLeft;
	//}
	//// Move Entity position
	move(velocity.x, velocity.y);
}

void EnemyEntity::setExpiredEntity()
{
	dyingEntity = true;
}

void EnemyEntity::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	if (std::dynamic_pointer_cast<PlayerEntity>(eCollisioning))
	{
		eCollisioning->setExpiredEntity();
	}
}

void EnemyEntity::update()
{
	updateVelocity();
	


	// If the enemy has died:
	if (!dyingEntity)
	{
		// If there is speed we must update animation sprite every X time
		if (GameTime::getTimeNow() - animLastTic > frameSpeed)
		{
			setTextureRect(enemyTexture->getWalkingSprite(lookingDir, currentFrame, enemyType));
			currentFrame = (currentFrame + 1) % walkFrames;
			animLastTic = GameTime::getTimeNow();
		}
	}
	else
	{
		if (currentFrame == deathFrames - 1 && GameTime::getTimeNow() - animLastTic > frameSpeed)
		{
			expiredEntity = true;
		}

		else if (GameTime::getTimeNow() - animLastTic > frameSpeed)
		{
			setTextureRect(enemyTexture->getDeathSprite(currentFrame, enemyType));
			currentFrame = (currentFrame + 1) % deathFrames;
			animLastTic = GameTime::getTimeNow();
		}
	}
}

sf::FloatRect EnemyEntity::getGlobalBounds() const
{
	sf::FloatRect dim = sf::Sprite::getGlobalBounds();
	return sf::FloatRect(dim.left + 6, dim.top + 3, dim.width - 6, dim.height - 6);
}

Balloon::Balloon() : EnemyEntity()
{
	enemyType = balloon;
	//move(500, 100);
}

Ice::Ice() : EnemyEntity()
{
	enemyType = ice;
	//move(200, 200);
}

Barrel::Barrel() : EnemyEntity()
{
	enemyType = barrel;
	//move(120, 500);
}
Coin::Coin() : EnemyEntity()
{
	enemyType = coin;
	//move(500, 450);
}

Blob::Blob() : EnemyEntity()
{
	enemyType = blob;
	//	move(400, 300);
}

Ghost::Ghost() : EnemyEntity()
{
	enemyType = ghost;
	//	move(200, 400);
}

Hypo::Hypo() : EnemyEntity()
{
	enemyType = hypo;
	//	move(400,600);
}
