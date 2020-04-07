#pragma once
#include "../Textures/WallTexture.h"
#include "Entity.h"

class BrickWall : public Entity {
public:
	WallTexture* wallTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int explosionCounter;
	bool isDestroyed = false;
	
	BrickWall(int &xloc, int &yloc) : Entity() {
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

	void update() override {
		if (isDestroyed) {
			spriteCounter++;
			spriteCounter %= spriteSpeed;
			if (spriteCounter == 0) {
				if (actualFrame == 7) {
					expiredEntity = true;
				}
				else {
					actualFrame = (actualFrame + 1) % 8;
					setTextureRect(wallTexture->getRectWall(actualFrame));
				}
			}
		}
	}

	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override{
		if(std::dynamic_pointer_cast<EnemyEntity>(eCollisioning)){
			//Puede atravesar????
			Entity::onCollission(eCollisioning, colT);
		}else{
			Entity::onCollission(eCollisioning, colT);
		}
	}


	void setExpiredEntity() override {
		isDestroyed = true;
	}
};
