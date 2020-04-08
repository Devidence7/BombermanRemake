#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "../GameMaths/GeometryMaths.hpp"
#include "../Exceptions/ExceptionsGame.hpp"
#include "../Logic/Random.h"

class Entity : public sf::Sprite
{
protected:
	double baseSpeed = 1;  // Base Entity Speed
	sf::Vector2f velocity; // Speed of the Entity right now.
	//sf::Vector2f size; // Size of the Entity
	bool expiredEntity = false; // Mark the entity as expired to be delete

	// Can be destroyed:
	bool isFireDestroyable = false;
	bool fireCanGoThroght = false;

	bool canHurtPlayers;  // Can hurt players:
	bool canHurtEntities; // Can hurt the playes, walls, enemies and objects:

	bool collisioner = true; //default: cant through
	bool onCollision = false; //if just have collision

public:
	Entity() : Sprite() {}

	virtual sf::FloatRect getGlobalBounds() const
	{
		return this->sf::Sprite::getGlobalBounds();
	}

	virtual sf::Vector2f getCenterPosition()
	{
		sf::FloatRect p = getGlobalBounds();
		return sf::Vector2f(p.left + p.width / 2, p.top + p.height / 2);
	}

	virtual bool isCollisioner(){
		return this->collisioner;
	}

	virtual void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT )
	{
		sf::Vector2f position = eCollisioning->getPosition();
		switch (colT)
		{
		case HORIZONTAL:
			eCollisioning->setCollision();
			position.x += this->moveOnX(eCollisioning);
			break;
		case VERTICAL:
			eCollisioning->setCollision();
			position.y += this->moveOnY(eCollisioning);
			break;
		case CORNER:
			eCollisioning->setCollision();
			position += this->moveCircleOverCorner(eCollisioning);
			break;
		
		default:
			break;
		}
		eCollisioning->setPosition(position);
	}
	

	void setCollision()
	{
		onCollision = true;
	}

	bool collision(const Entity &e) const
	{
		return this->Entity::getGlobalBounds().intersects(e.getGlobalBounds());
	}

	virtual void update()
	{
		throw NoImplementedException("Intentando ejecutar UPDATE Entity");
	}

	virtual bool getExpiredEntity()
	{
		return expiredEntity;
	}

	virtual void setExpiredEntity()
	{
		expiredEntity = true;
	}

	bool getIsFireDestroyable()
	{
		return isFireDestroyable;
	}

	bool getFireCanGoThroght()
	{
		return fireCanGoThroght;
	}


	// PRE: this esta en colision con eCollisioning
	//Devuelve cuanto penetra eCollisioning en this en el eje X; Y se considera centrado 
	float moveOnX(const std::shared_ptr<Entity> eCollisioning)
	{
		
		sf::Vector2f position = eCollisioning->getCenterPosition();
		sf::FloatRect body = eCollisioning->getGlobalBounds();
		sf::Vector2f position2 = this->getCenterPosition();
		sf::FloatRect body2 = this->getGlobalBounds();
		float x = calculate_penetration(body.width / 2, body2.width / 2, abs(position.x - position2.x)) + 1;
		if (position.x < position2.x)
		{
			x = -x;
		} //retroceder
		return x;
	}


	// PRE: this esta en colision con eCollisioning
	//Devuelve cuanto penetra eCollisioning en this en el eje Y; X se considera centrado
	float moveOnY(const std::shared_ptr<Entity> eCollisioning)
	{
		
		sf::Vector2f position = eCollisioning->getCenterPosition();
		sf::FloatRect body = eCollisioning->getGlobalBounds();
		sf::Vector2f position2 = this->getCenterPosition();
		sf::FloatRect body2 = this->getGlobalBounds();

		float y = calculate_penetration(body.height / 2, body2.height / 2, abs(position.y - position2.y)) + 1;
		if (position.y < position2.y)
		{
			y = -y;
		} //retroceder
		return y;
	}

	

	//eCirle tiene una colision con eRect. El centro de no está eCricle no está dentro de eRect
	//Devuelve el la direccion (cuyo modulo es la cantidad) al que se tiene que mover la entidad eCircle que esta colisionando con eRect
	inline sf::Vector2f moveCircleOverCorner(std::shared_ptr<Entity> eCircle)
	{

		sf::Vector2f CenterCircle = eCircle->getCenterPosition();
		float r_Circle  = eCircle->getGlobalBounds().width/2;
		
		sf::Vector2f centerRect = this->getCenterPosition();
		sf::FloatRect bodyRect = this->getGlobalBounds();

		sf::Vector2f dir_centros = centerRect - CenterCircle;
		float distanciaCentros = moduleVector(dir_centros);
		dir_centros = dir_centros / distanciaCentros;
		float intersec_x;
		float intersec_y;

		//Buscar coordenadas referecia
		sf::Vector2f puntoReferenciaRect(bodyRect.left, bodyRect.top);

		if (CenterCircle.y > centerRect.y)
		{
			puntoReferenciaRect.y += bodyRect.height;
		}
		if (CenterCircle.x > centerRect.x)
		{
			puntoReferenciaRect.x += bodyRect.width;
		}
		intersec_x = intersectPoint_planeVector(CenterCircle, dir_centros, sf::Vector2f(1, 0), puntoReferenciaRect);
		intersec_y = intersectPoint_planeVector(CenterCircle, dir_centros, sf::Vector2f(0, 1), puntoReferenciaRect);

		float referencia = 0;
		float intersec = 0;
		if (CenterCircle.x >= bodyRect.left && CenterCircle.x <= bodyRect.left + bodyRect.width)
		{
			referencia = intersec_y;
		}
		else if (CenterCircle.y >= bodyRect.top && CenterCircle.y <= bodyRect.top + bodyRect.height)
		{
			referencia = intersec_x;
		}
		else
		{ //Esquina
			referencia = intersec_x > intersec_y ? intersec_x : intersec_y;
		}
		intersec = ((distanciaCentros - referencia) + r_Circle) - distanciaCentros;
		
		return (-dir_centros) * intersec;
	}


	void drawEntityHitbox(sf::RenderWindow &w)
	{
		sf::FloatRect dim = this->getGlobalBounds();
		sf::VertexArray lines(sf::Lines, 2);

		// Left
		lines[0].position = sf::Vector2f(dim.left, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		// top
		lines[0].position = sf::Vector2f(dim.left, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		lines[0].position = sf::Vector2f(dim.left, dim.top + dim.height);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		lines[0].position = sf::Vector2f(dim.left + dim.width, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);
	}

};
