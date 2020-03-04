#pragma once
#include <SFML/Graphics.hpp>

#include "../Textures/texture.h"

class Entity : public sf::Sprite {
public:
	float baseSpeed;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	sf::Vector2f size; // Size of the Entity

	/* 
	*  Constructor of Entity
	*/
	Entity() : Sprite() {

	}

};
