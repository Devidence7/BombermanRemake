#include "../Include/EntitiesInclude.hpp"


Pillar::Pillar(int &x, int &y, int stage) : Entity()
{
	// Set pillar position
	setPosition(sf::Vector2f(SIZE_PILLAR * x, SIZE_PILLAR * y));

	this->stage = stage;

	// Texture Controller
	wallTexture = &TextureStorage::getlevel1WallTexture();
	// Set starting sprite
	setTextureRect(wallTexture->getRectPillar(stage));
	// Set sprite Sheet texture
	setTexture(wallTexture->getTexture());
}
void Pillar::update() {}
