#pragma once
#include "BombTexture.h"
#include "PlayerTexture.hpp"
#include "WallTexture.h"

class TextureStorage {
	PlayerTexture playerTexture;
	WallTexture level1WallTexture;
	BombTexture bombTexture;

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
	
};
