#include "TextureVector.h"

sf::Texture &TextureVector::getTexture()
{
	return texture;
}

sf::IntRect &TextureVector::getDefaultIntRect()
{
	return frames[0];
}