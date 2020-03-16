#pragma once
#include "BombTexture.h"
#include "EnemyTexture.h"
#include "PlayerTexture.hpp"
#include "WallTexture.h"
#include "FireTexture.h"
#include "PowerUpTexture.h"

class TextureStorage {
	static PlayerTexture playerTexture;
	static WallTexture level1WallTexture;
	static BombTexture bombTexture;
	static FireTexture fireTexture;
	static EnemyTexture enemyTexture; 
	static PowerUpTexture powerUpTexture;

public:

	TextureStorage()
	{
		
	}

	static PlayerTexture& getPlayerTexture() {
		return playerTexture;
	}

	static WallTexture& getlevel1WallTexture() {
		return level1WallTexture;
	}

	static BombTexture& getBombTexture() {
		return bombTexture;
	}

	static FireTexture& getFireTexture() {
		return fireTexture;
	}

	static EnemyTexture& getEnemyTexture() {
		return enemyTexture;
	}

	static PowerUpTexture& getPowerUpTexture() {
		return powerUpTexture;
	}
};


PlayerTexture TextureStorage::playerTexture;
WallTexture TextureStorage::level1WallTexture;
BombTexture TextureStorage::bombTexture;
FireTexture TextureStorage::fireTexture;
EnemyTexture TextureStorage::enemyTexture;


