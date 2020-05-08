#pragma once
#include "BombTexture.h"
#include "EnemyTexture.h"
#include "PlayerTexture.hpp"
#include "WallTexture.h"
#include "FireTexture.h"
#include "PowerUpTexture.h"

class TextureStorage {
	static PlayerTexture playerTexture;
	static PlayerColor playerColor;
	static WallTexture level1WallTexture;
	static BombTexture bombTexture;
	static FireTexture fireTexture;
	static EnemyTexture enemyTexture; 
	static PowerUpTexture powerUpTexture;


public:

	TextureStorage();

	static PlayerTexture& getPlayerTexture();

	static PlayerColor& getPlayerColor();

	static WallTexture& getlevel1WallTexture();

	static BombTexture& getBombTexture();

	static FireTexture& getFireTexture();

	static EnemyTexture& getEnemyTexture();

	static PowerUpTexture& getPowerUpTexture();

	
};