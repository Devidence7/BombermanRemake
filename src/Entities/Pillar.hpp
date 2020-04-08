#pragma once

#include "Entity.h"
#include "../Textures/TextureStorage.h"
#include "../Textures/WallTexture.h"


class Pillar : public Entity {

	WallTexture* wallTexture;

public:
	Pillar(int &x, int &y);
	void update() override;
};