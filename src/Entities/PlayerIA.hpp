
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
	sf::Vector2f lastPositionKnowed;
	PatrolState p;
	int IAPlayer;

	/*,pos
	Constructor of Entity
	*/
	PlayerIAEntity(PlayerControls &playerControls,int posX,int posY, int _team,int IA) : PlayerEntity(playerControls, _team,posX,posY){
		colorPath = sf::Color(Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255));
		IAPlayer=IA;
		cout<<"POS DE LA IA: "<<posX<<" "<<posY<<endl;
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
	bool updatePlayer(int posX,int posY) override;

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
	void decildetState();

	float getIntersetActionPU(){
		return sg.intersetACtionPU;
	}
	float getIntersetNerfPU(){
		return sg.intersetNerfPU;
	}
	float getIntersetBuffPE(){
		return sg.intersetBuffPU;
	}

	float getIntersetDestroyWalls(){
		return sg.bonusDestroyWalls;
	}
};