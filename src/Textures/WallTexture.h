#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"

class WallTexture : public TextureVector {

	public:
	WallTexture(){
		if(!this->texture.loadFromFile("../textures/blocks.png")){
			throw ExceptionLoadImage("Imagen no cargada: ../textures/blocks.png");
		}
		this->frames.push_back(sf::IntRect(0,0,48,48));
		this->frames.push_back(sf::IntRect(51,0,99,48));
	}

	sf::Texture &getTexture(){
		return this->texture;
	}

	sf::IntRect &getRectWall(){
		return this->frames[1];
	}

	sf::IntRect &getRectPillar(){
		return this->frames[0];
	}

};
