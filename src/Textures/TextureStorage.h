#pragma once
#include "BombTexture.h"
#include "PlayerTexture.hpp"
#include "WallTexture.h"
#include "FireTexture.h"

class TextureStorage {
	PlayerTexture playerTexture;
	WallTexture level1WallTexture;
	BombTexture bombTexture;
	FireTexture fireTexture;
	EnemyTexture enemyTexture; 

public:

	TextureStorage()
	{
		
	}

	PlayerTexture& getPlayerTexture() {
		return playerTexture;
	}

	WallTexture& getlevel1WallTexture() {
		return level1WallTexture;
	}

	BombTexture& getBombTexture() {
		return bombTexture;
	}

	FireTexture& getFireTexture() {
		return fireTexture;
		
	}

	EnemyTexture& getEnemyTexture() {
		return enemyTexture;
	}
};
