#pragma once

#include "Entity.h"
#include "../Textures/TextureStorage.h"
#include "../Textures/WallTexture.h"


class Pillar : public Entity {

	WallTexture* wallTexture;
	int stage;

public:
	Pillar(int &x, int &y, int stage);
	void update() override;
};