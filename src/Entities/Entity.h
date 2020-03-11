#pragma once
#include <SFML/Graphics.hpp>


#include "../Exceptions/ExceptionsGame.hpp"
#include "../Phisics/collider2d.hpp"


class Entity : public sf::Sprite {
public:
	float baseSpeed = 1;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	sf::Vector2f size; // Size of the Entity
	
	bool expiredEntity = false; // Mark the entity as expired to be deleted


	Entity() : Sprite() {}

	Entity(TextureVector& tv) : Sprite() {
		// Set starting sprite
		setTextureRect(tv.getDefaultIntRect());
		// Set sprite Sheet texture
		setTexture(tv.getTexture());
	}
	

	virtual sf::FloatRect getGlobalBounds() const
	{
		return this->sf::Sprite::getGlobalBounds();
	}

	bool collision(const Entity & e) const{
		return this->Entity::getGlobalBounds().intersects(e.getGlobalBounds());
	}

	virtual void update(void) {
		throw NoImplementedException("Intentando ejecutar UPDATE Entity");
	}
};
