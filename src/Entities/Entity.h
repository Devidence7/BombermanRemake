#pragma once
#include <SFML/Graphics.hpp>

#include "../Phisics/collider2d.hpp"


class Entity : public sf::Sprite {
public:
	float baseSpeed;// Base Entity Speed 
	sf::Vector2f velocity; // Speed of the Entity right now.
	sf::Vector2f size; // Size of the Entity
	
	Collider2d *body =nullptr;
	bool expiredEntity = false; // Mark the entity as expired to be deleted

	bool hasCollider()const{
		return body != nullptr;
	}

	virtual sf::Vector2f getPosition() const{
		return this->sf::Sprite::getPosition();
	}

	virtual sf::FloatRect getGlobalBounds() const
	{
		//if(this->body != nullptr){
		//	return body->getRect();
		//}
		return this->sf::Sprite::getGlobalBounds();
	}

	bool collision(const Entity & e) const{
		return this->Entity::getGlobalBounds().intersects(e.getGlobalBounds());
	}

	Entity(){}
	Entity(Collider2d &col) : body(&col){}
	Entity(Collider2d *col) : body(col){}

	virtual void update(void) {
		throw NoImplementedException("Intentando ejecutar UPDATE Entity");
	}
};
