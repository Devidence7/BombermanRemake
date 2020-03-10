#pragma once
#include <SFML/Graphics.hpp>

#include "../Phisics/collider2d.hpp"


class Entity : public sf::Sprite {
public:
	float baseSpeed;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	sf::Vector2f size; // Size of the Entity
	
	bool expiredEntity = false; // Mark the entity as expired to be deleted

	virtual sf::FloatRect getGlobalBounds() const
	{
		return this->sf::Sprite::getGlobalBounds();
	}

	bool collision(const Entity & e) const{
		return this->Entity::getGlobalBounds().intersects(e.getGlobalBounds());
	}

	Entity() : Sprite(){}

	virtual void update(void) = 0;
};
