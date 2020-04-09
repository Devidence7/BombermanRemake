#include "../Include/EntitiesInclude.hpp"


Pillar::Pillar(int &x, int &y) : Entity()
{
	// Set pillar position
	setPosition(sf::Vector2f(SIZE_PILLAR * x, SIZE_PILLAR * y));

	// Texture Controller
	wallTexture = &TextureStorage::getlevel1WallTexture();
	// Set starting sprite
	setTextureRect(wallTexture->getRectPillar());
	// Set sprite Sheet texture
	setTexture(wallTexture->getTexture());
}
void Pillar::update() {}
