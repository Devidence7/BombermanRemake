#pragma once
#include "Entity.h"

class Bomb : public Entity {
public:
	BombTexture* bombTexture;
	int spriteCounter;
	int spriteSpeed;
	int actualFrame;
	int spriteFrames;

	int explosionCounter;
	Bomb() : Entity() {
		spriteCounter = 0;
		spriteSpeed = 15;
		actualFrame = 0;
		spriteFrames = 3;
		explosionCounter = 0;

		// Texture Controller:
		bombTexture = &TextureStorage::getBombTexture();
		// Set starting sprite
		setTextureRect(bombTexture->getFrame(0));
		// Set sprite Sheet texture
		setTexture(bombTexture->getTexture());
	}

	void update() {

	}
};