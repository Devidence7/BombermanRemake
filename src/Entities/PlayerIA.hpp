
#pragma once
#include "Player.h"
#include "../Utils/OmittedAreas.hpp"

class PlayerIAEntity : public PlayerEntity
{
	sf::Color colorPath;
public:
	ANode_Ptr currentMovement;
	std::list<ANode_Ptr> movements;
	int IAPlayer;

	/*
	Constructor of Entity
	*/
	PlayerIAEntity(PlayerControls &playerControls, int _team,int IA) : PlayerEntity(playerControls, _team){
		colorPath = sf::Color(Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255), Random::getIntNumberBetween(0, 255));
		IAPlayer=IA;
	}

//	sf::FloatRect getGlobalBounds() const override; Modificar??

//	void update() override;
	void setCollision(std::shared_ptr<Entity> e) override;
	void drawMovements(sf::RenderWindow &w);
//	void realizeActions();
	void generateMovements();
	bool updateVelocity();
	void updateMovement();
	bool playerActions() override;
	/*
	 * Update player position.
	 */
	bool updatePlayer() override;

//	void invertControls(); ??

};