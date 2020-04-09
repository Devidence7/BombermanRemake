#pragma once
#include "../Textures/WallTexture.h"
#include "../Textures/TextureStorage.h"
#include "Entity.h"
#include "Enemy.h"

class BrickWall : public Entity {
public:
	WallTexture* wallTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int explosionCounter;
	bool isDestroyed = false;
	
	BrickWall(int &xloc, int &yloc);
	void update() override;

	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;
	void setExpiredEntity() override;
};
