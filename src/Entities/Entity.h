#pragma once
#include <SFML/Graphics.hpp>


#include "../Exceptions/ExceptionsGame.hpp"


class Entity : public sf::Sprite {
protected:
	float baseSpeed = 1;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	//sf::Vector2f size; // Size of the Entity
	bool expiredEntity = false; // Mark the entity as expired to be deleted

public:
	Entity() : Sprite() {}

	virtual sf::FloatRect getGlobalBounds() const
	{
		return this->sf::Sprite::getGlobalBounds();
	}

	bool collision(const Entity & e) const{
		return this->Entity::getGlobalBounds().intersects(e.getGlobalBounds());
	}

	virtual void update() {
		throw NoImplementedException("Intentando ejecutar UPDATE Entity");
	}

	virtual bool getExpiredEntity() {
		return expiredEntity;
	}

	virtual void setExpiredEntity() {
		expiredEntity = true;
	}
	
};
