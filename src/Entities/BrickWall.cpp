#include "../Include/EntitiesInclude.hpp"


BrickWall::BrickWall(int &xloc, int &yloc) : Entity()
{
	isFireDestroyable = true;

	// Set coordinates:
	setPosition(xloc * SIZE_PILLAR, yloc * SIZE_PILLAR);

	// Texture Controller
	wallTexture = &TextureStorage::getlevel1WallTexture();
	// Set starting sprite
	setTextureRect(wallTexture->getDefaultIntRect());
	// Set sprite Sheet texture
	setTexture(wallTexture->getTexture());
}

void BrickWall::update()
{
	if (isDestroyed)
	{ 
		if (GameTime::getTimeNow() - spriteStartTime > expiredTime) {
			expiredEntity = true;
		}
		else if (GameTime::getTimeNow() - spriteLastFrameTime > spriteSpeed) {
			spriteLastFrameTime = GameTime::getTimeNow();
			actualFrame = (actualFrame + 1) % spriteFrames;
			setTextureRect(wallTexture->getRectWall(actualFrame));
		}	
	}
}
 

bool BrickWall::isColliderWith(std::shared_ptr<Entity> eCollisioning) {
	return eCollisioning->CanThroughWall();
}

void BrickWall::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	if (std::dynamic_pointer_cast<EnemyEntity>(eCollisioning))
	{
		//Puede atravesar????
		Entity::onCollission(eCollisioning, colT);
	}
	else
	{
		Entity::onCollission(eCollisioning, colT);
	}
}

void BrickWall::setExpiredEntity()
{
	spriteStartTime = GameTime::getTimeNow();
	spriteLastFrameTime = GameTime::getTimeNow();
	isDestroyed = true;
}
