#pragma once
#include "Entity.h"
#include "Player.h"
#include "../Textures/PowerUpTexture.h"
#include "../GameEngine.hpp"
#include "../Textures/TextureStorage.h"
/*
class Teleporter : public Entity {
public:
	Texture* teleporterTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int spriteFrames;
	Teleporter();

	void open();

	void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT) override;

	virtual void setPlayerStatus(Game &game);
};
*/