#include "Entity.h"


class Pillar : public Entity {

	WallTexture* wallTexture;

public:
	Pillar(int &x, int &y) : Entity() {
		// Set pillar position
		setPosition(sf::Vector2f(SIZE_PILLAR * x, SIZE_PILLAR * y));

		// Texture Controller
		wallTexture = &TextureStorage::getlevel1WallTexture();
		// Set starting sprite
		setTextureRect(wallTexture->getRectPillar());
		// Set sprite Sheet texture
		setTexture(wallTexture->getTexture());
	}
	void update() override{}

};