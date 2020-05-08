#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
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

	ActionsAvalible actionAvaible = ActionsAvalible::NONE_ACTION;
	int score = 0;
	std::list<std::shared_ptr<Entity>> BombsAsociated;
	std::shared_ptr<Entity> BombTaked = nullptr;

	sf::Vector2f positionObjetive;


public:
	int rangoVision = 0; // Rango de celdas cuadrada que puede ver al jugador
	sf::Vector2i siteRespectPlayer; // Sitio respecto jugador a donde ira a buscar un camino si existe
	int numConsecutiveMovements = 5;
	int numMovenet = 0;
	TypePositionRelative typePosIA = TypePositionRelative::OBJETIVE_POSITION;
	TypeSeekIA typeSeek = TypeSeekIA::SECOND_BEST_PATH;
	int longMorePAth = 3;

	Entity();
	virtual sf::Vector2f getCenterPosition();

	virtual bool isColliderWith(std::shared_ptr<Entity> eCollisioning);

	bool CanHurtPlayer();

	sf::Vector2f getVelocity(){
		return velocity;
	}
	void setVelocity(sf::Vector2f &vel){
		velocity.x = vel.x * baseSpeed;
		velocity.y = vel.y * baseSpeed;
	}

	virtual void onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT );
	void setCollision();

	bool collision(const Entity &e) const;

	virtual void update();

	virtual bool getExpiredEntity();

	virtual void setExpiredEntity();

	bool getIsFireDestroyable();

	bool getFireCanGoThroght();

	bool CanThroughWall(){
		return this->canThroughWall;
	}

	virtual void setCanThroughBomb(bool _ctb = true){
		canThroughBomb = _ctb;
	}

	void setCanThroughWall(bool _canThroughWall){
		this->canThroughWall = _canThroughWall;
	}

	void setAction(ActionsAvalible _action){
		if(actionAvaible == ActionsAvalible::REMOTE_BOMB && _action != actionAvaible){
			changeBombs2time();
		}
		actionAvaible =_action;
	}

	bool CanKickBom(){
		return ActionsAvalible::KICK_BOM == actionAvaible;
	}

	bool CanGrabBomb(){
		return this->actionAvaible == ActionsAvalible::GRAB_BOMB ;
	}
	

	bool CanThroughBomb(){
		return canThroughBomb || actionAvaible == ActionsAvalible::THROUGH_BOMB;
	}

	virtual void setJumpingBomb(){
		//TODO: ???
		return;
	}

	void setObjetive(sf::Vector2f pos);


	virtual sf::FloatRect getGlobalBounds() const;
	virtual sf::FloatRect getGlobalBoundsCollisioner() const;
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

	void incrementScore(int S){
		score += S;
	}

	virtual void bombExploted(std::shared_ptr<Entity> b);

	ActionsAvalible getActionAssigned(){
		return actionAvaible;
	}

	void AssignBomb(std::shared_ptr<Entity> b){
		BombsAsociated.push_back(b);
	}
	void changeBombs2time();

};
