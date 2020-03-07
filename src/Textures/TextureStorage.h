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

public:
	TextureStorage()
	{
		playerTexture = PlayerTexture();
		bombTexture = BombTexture();
	}

	PlayerTexture &getPlayerTexture()
	{
		return playerTexture;
	}

	BombTexture& getBombTexture() {
		return bombTexture;
	}

	FireTexture& getFireTexture() {
		return fireTexture;
		;
	}
	
};
