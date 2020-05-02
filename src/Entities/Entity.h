#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

#include "../GameMaths/GeometryMaths.hpp"
#include "../Include/global.hpp"
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

	bool canThroughWall = false; //puede atravesar paredes
	bool canThroughBomb = false; //puede atravesar bombas

	bool canKickBom = false;
	bool canGrabBomb = false;

	std::list<std::shared_ptr<Entity>> BombsAsociated;
	std::shared_ptr<Entity> BombTaked = nullptr;

public:
	Entity();
	virtual sf::Vector2f getCenterPosition();

	virtual bool isCollisioner();

	bool CanHurtPlayer();

	virtual void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT );
	void setCollision();

	bool collision(const Entity &e) const;

	virtual void update();

	virtual bool getExpiredEntity();

	virtual void setExpiredEntity();

	bool getIsFireDestroyable();

	bool getFireCanGoThroght();

	bool CanKickBom(){
		return this->canKickBom;
	}

	void setCanKickBom(bool _canKickBom){
		canKickBom = _canKickBom;
	}

	bool CanThroughWall(){
		return this->canThroughWall;
	}

	void setCanThroughWall(bool _canThroughWall){
		this->canThroughWall = _canThroughWall;
	}

	bool CanGrabBomb(){
		return canGrabBomb;
	}

	void setCanGrabBomb(bool _canGrabBomb){
		canGrabBomb = _canGrabBomb;
	}

	bool CanThroughBomb()	{
		return this->canThroughBomb;
	}

	virtual void setJumpingBomb(){
		//TODO: ???
		return;
	}

	void setCanThroughBomb(bool _canThroughBomb){
		this->canThroughBomb = _canThroughBomb;
	}

	virtual sf::FloatRect getGlobalBounds() const;
	// PRE: this esta en colision con eCollisioning
	//Devuelve cuanto penetra eCollisioning en this en el eje X; Y se considera centrado 
	float moveOnX(const std::shared_ptr<Entity> eCollisioning);

	// PRE: this esta en colision con eCollisioning
	//Devuelve cuanto penetra eCollisioning en this en el eje Y; X se considera centrado
	float moveOnY(const std::shared_ptr<Entity> eCollisioning);

	

	//eCirle tiene una colision con eRect. El centro de no está eCricle no está dentro de eRect
	//Devuelve el la direccion (cuyo modulo es la cantidad) al que se tiene que mover la entidad eCircle que esta colisionando con eRect
	inline sf::Vector2f moveCircleOverCorner(std::shared_ptr<Entity> eCircle);


	void drawEntityHitbox(sf::RenderWindow &w);

};
