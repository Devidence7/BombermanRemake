#include "TeleporterTexture.h"
#include "../Exceptions/ExceptionsGame.hpp"

TeleporterTexture::TeleporterTexture() {
	if (!this->texture.loadFromFile("../textures/Entities/Teleport.png")) {
		throw ExceptionLoadImage("Imagen no cargada: ../textures/Entities/Teleport.png");
	}
	int frameSizex = 48;
	int frameSizey = 48;

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 5; i++) {
			this->frames.push_back(sf::IntRect(0 + frameSizex * i, 0 + frameSizey * j, frameSizex, frameSizey));
		}
		for (int i = 3; i > 0; i--) {
			this->frames.push_back(sf::IntRect(0 + frameSizex * i, 0 + frameSizey * j, frameSizex, frameSizey));
		}
	}
}

sf::IntRect& TeleporterTexture::getFrame(int i, int state) {
	return frames[state * 8 + i];
}