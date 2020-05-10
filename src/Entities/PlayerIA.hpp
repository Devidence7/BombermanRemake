
#pragma once
#include "Player.h"
#include "../Utils/OmittedAreas.hpp"
#include "../PseudoPPDL/ParserPPDL.hpp"
#include "../PseudoPPDL/Estados.hpp"
#include "../Utils/IAglobal.hpp"


class PlayerIAEntity : public PlayerEntity
{
	sf::Color colorPath;
public:
	ANode_Ptr currentMovement;
	std::list<ANode_Ptr> movements;
	sf::Vector2i currentObjetive;
	StateGenerator sg;
	StateIA currentState = StateIA::NON_OBJETIVE;
	PatrolState p;
	int IAPlayer;
	/*
	Constructor of Entity
	*/
	PlayerIAEntity(PlayerControls &playerControls, int _team,int IA) : PlayerEntity(playerControls, _team){
		colorPath = sf::Color(Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255));
		IAPlayer=IA;
	}

	void createStateGenerator(string path){
		sg.readIA(path);
	}
//	sf::FloatRect getGlobalBounds() const override; Modificar??

//	void update() override;
	void setCollision(std::shared_ptr<Entity> e) override;
	void drawMovements(sf::RenderWindow &w);
//	void realizeActions();
	//void generateMovements();
	bool updateVelocity();
	void updateMovement();
	bool playerActions() override;
	/*
	 * Update player position.
	 */
	bool updatePlayer() override;

//	void invertControls(); ??


	////////IA FUNCTION

	void startStates();
	void seekAndDestroyEnemy();
	void updateState();
	void generatePathStates();
	bool updatePatrolState();
	bool updateKillState();
	bool updatePerseguirState();
	bool updateRunawayState();
	void putABomb();
};