#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Exceptions/ExceptionsGame.hpp"
#include "TextureVector.h"

#define sizeWall 48
#define sizePillar 48

class WallTexture : public TextureVector {
	public:
	WallTexture(){
		if(!this->texture.loadFromFile("../textures/Entities/blocks.png")){
			throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/blocks.png");
		}

		for(int i = 0; i < 8; i++)
		{
			this->frames.push_back(sf::IntRect(51 * i, 0, sizePillar, sizePillar));
		}
	}

	sf::IntRect& getDefaultIntRect() override {
		return frames[1];
	}

	sf::IntRect &getRectWall(int i){
		return this->frames[i];
	}

	sf::IntRect &getRectPillar(){
		return this->frames[0];
	}
};
