#pragma once
#include <SFML/Graphics.hpp>

class Entity : public sf::Sprite {
public:
	float baseSpeed;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	sf::Vector2f size; // Size of the Entity

	bool expiredEntity = false; // Mark the entity as expired to be deleted

	virtual void update(void) = 0;
};
