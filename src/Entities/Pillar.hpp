#pragma once

#include "Entity.h"
#include "../Textures/TextureStorage.h"
#include "../Textures/WallTexture.h"


class Pillar : public Entity {

	WallTexture* wallTexture;
	Sprite shadow;
	int stage;

public:
	Pillar(int &x, int &y, int stage);


	void drawShadow(sf::RenderWindow& window) override;
	void update() override;
};