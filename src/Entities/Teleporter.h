#pragma once
#include "Entity.h"
#include "Player.h"
#include "../Textures/TextureStorage.h"

class Teleporter : public Entity {
public:
	TeleporterTexture* teleporterTexture;

	double spriteCounter;
	const double spriteSpeed = 0.2;
	int currentFrame = 0;
	const int spriteFrames = 8;
	bool playerCross = false;

	enum TeleporterState {
		CLOSE = 0,
		OPEN = 1
	};

	TeleporterState teleporterState = TeleporterState::CLOSE;

	Teleporter(sf::Vector2f pos);

	void update();

	void onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT);

	void openTeleporter();
	void closeTeleporter();
};