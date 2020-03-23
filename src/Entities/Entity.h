#pragma once
#include <SFML/Graphics.hpp>


#include "../Exceptions/ExceptionsGame.hpp"


class Entity : public sf::Sprite {
protected:
	double baseSpeed = 1;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	//sf::Vector2f size; // Size of the Entity
	bool expiredEntity = false; // Mark the entity as expired to be delete

	// Can be destroyed:
	bool isFireDestroyable = false;
	bool fireCanGoThroght = false;

	bool canHurtPlayers;	// Can hurt players:
	bool canHurtEntities;	// Can hurt the playes, walls, enemies and objects:

public:
	Entity() : Sprite() {}

	virtual sf::FloatRect getGlobalBounds() const
	{
		return this->sf::Sprite::getGlobalBounds();
	}

	virtual sf::Vector2f getCenterPosition() {
		sf::FloatRect p = getGlobalBounds();
		return sf::Vector2f(p.left + p.width / 2, p.top + p.height / 2);
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

	bool getIsFireDestroyable() {
		return isFireDestroyable;
	}

	bool getFireCanGoThroght() {
		return fireCanGoThroght;
	}
	
};
