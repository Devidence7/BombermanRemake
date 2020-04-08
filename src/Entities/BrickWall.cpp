#include "../Include/EntitiesInclude.hpp"


BrickWall::BrickWall(int &xloc, int &yloc) : Entity()
{
	isFireDestroyable = true;

	spriteCounter = 0;
	spriteSpeed = 3;
	actualFrame = 0;
	explosionCounter = 0;

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
		spriteCounter++;
		spriteCounter %= spriteSpeed;
		if (spriteCounter == 0)
		{
			if (actualFrame == 7)
			{
				expiredEntity = true;
			}
			else
			{
				actualFrame = (actualFrame + 1) % 8;
				setTextureRect(wallTexture->getRectWall(actualFrame));
			}
		}
	}
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
	isDestroyed = true;
}
