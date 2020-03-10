#pragma once
#include "../Textures/WallTexture.h"
#include "Entity.h"

class BrickWall : public Entity {
public:
	WallTexture* wallTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int explosionCounter;
	bool isDestroyed = false;
	
	BrickWall(WallTexture& wt, int xloc, int yloc) : Entity() {
		spriteCounter = 0;
		spriteSpeed = 3;
		actualFrame = 0;
		explosionCounter = 0;

		// Set coordinates:
		setPosition(xloc, yloc);
		// Texture Controller:
		wallTexture = &wt;
		// Set starting sprite
		setTextureRect(wallTexture->getRectWall(1));
		// Set sprite Sheet texture
		setTexture(wallTexture->getTexture());
	}

	void update() override {
		if (isDestroyed) {
			spriteCounter++;
			spriteCounter %= spriteSpeed;
			if (spriteCounter == 0) {
				if (actualFrame == 7) {
					expiredEntity = true;
				}
				else {
					actualFrame = (actualFrame + 1) % 8;
					setTextureRect(wallTexture->getRectWall(actualFrame));
				}
			}
		}
	}
};