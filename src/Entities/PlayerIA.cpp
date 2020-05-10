#include "../Map/Level.hpp"
#include "../Include/EntitiesInclude.hpp"
#include "../PseudoPPDL/Estados.hpp"
#include "../Utils/IAFunctions.hpp"

//	sf::FloatRect getGlobalBounds() const override; Modificar??

//	void update() override;
void PlayerIAEntity::setCollision(std::shared_ptr<Entity> col){
	this->Entity::setCollision(col);
	if(col != nullptr){
		OmittedAreas.push_back(OmittedArea(getMapCoordinates(col->getCenterPosition())));
	}
}

//	void realizeActions();

bool PlayerIAEntity::playerActions(){

}
/*
* Update player position.
*/

void PlayerIAEntity::drawMovements(sf::RenderWindow &w)
{
	sf::FloatRect dim = this->getGlobalBounds();
	for (ANode_Ptr &an : movements)
	{
		sf::RectangleShape rece;
		rece.setSize(sf::Vector2f((dim.width / 2), (dim.height / 2)));
		rece.setFillColor(colorPath);
		sf::Vector2i posi = an->getPosition();
		sf::Vector2f posf = MapCoordinates2GlobalCoorCenter(an->getPosition());
		//std::cout << "pos " << posi.x << "( " << posf.x<< ")" << posi.y << "( " << posf.y<< ")" << std::endl;
		posf -= sf::Vector2f(dim.width / 4, dim.height / 4);
		rece.setPosition(posf);
		w.draw(rece);
	}
}


// void PlayerIAEntity::generateMovements(){

// 	std::vector<sf::Vector2i> ob;
// 	ob.push_back(currentObjetive);
// 	if(!pathFinding(getMapCoordinates(getCenterPosition()), ob, movements, me, TypeSeekIA::BEST_PATH, false)){
// 		velocity = sf::Vector2f(0,0);
// 		movements.clear();
// 		currentMovement = std::make_shared<ANode>(ANode(getMapCoordinates(getCenterPosition()), sf::Vector2i(0,0), getMapCoordinates(getCenterPosition()), 0));
// 		movements.push_back(currentMovement);
// 	}
// }

void PlayerIAEntity::generatePathStates(){
	bool generatedNewObj = false;
	switch (currentState)
	{
	case StateIA::PATROL:
		generatedNewObj = updatePatrolState();
		break;
	case StateIA::PERSEGUIR:
		generatedNewObj = updatePerseguirState();
	default:
		break;
	}
}

void PlayerIAEntity::updateMovement(){
	
	if(movements.size() < 1){
		currentState = StateIA::NON_OBJETIVE;
		currentMovement = std::make_shared<ANode>(ANode(getMapCoordinates(getCenterPosition()), sf::Vector2i(0,0), getMapCoordinates(getCenterPosition()), 0));
	}
	else if (currentMovement == nullptr || checkArrivePosition(this->getCenterPosition(), currentMovement->getPosition(), currentMovement->getAction()))
	{ //Si esta en posicions
		currentMovement = movements.front();
		movements.pop_front();
		numMovenet --;
	}
}

bool PlayerIAEntity::updateVelocity(){
	velocity.x = 0;
	velocity.y = 0;
	if(currentMovement == nullptr){
		return false;
	}
	sf::Vector2f nextPosition = MapCoordinates2GlobalCoorCenter(currentMovement->getPosition());
	sf::Vector2f currentPosition = getCenterPosition();
	sf::Vector2f dir = nextPosition - currentPosition;
	double moveTime = 0;
	if (lastMovementTime) {
		moveTime = (GameTime::getTimeNow() - lastMovementTime) * 60;
	}
	lastMovementTime = GameTime::getTimeNow();
	velocity = normalize(dir);

	if(abs(dir.x) < 1 && abs(dir.y) < 1){
		velocity.x = 0;		
		velocity.y = 0;		

	}else if(abs(velocity.x) < 0.1){
		velocity.x = 0;
		velocity.y = velocity.y/abs(velocity.y);
	}else if(abs(velocity.y) < 0.1){
		velocity.y = 0;
		velocity.x = velocity.x/abs(velocity.x);
	}

	velocity.x *=  baseSpeed * speedBoost * moveTime;
	velocity.y *=  baseSpeed * speedBoost * moveTime;
	if (velocity.x != 0) {
		if (velocity.x > 0) {
			lastMovement = LookingAt::right;
		}
		else {
			lastMovement = LookingAt::left;
		}
	}
	else if (velocity.y != 0) {
		if (velocity.y > 0) {
			lastMovement = LookingAt::down;
		}
		else {
			lastMovement = LookingAt::up;
		}
	}

}

void PlayerIAEntity::seekAndDestroyEnemy(){
	
}


bool PlayerIAEntity::updatePatrolState(){
	if(isOn(this->getCenterPosition(), MapCoordinates2GlobalCoorCenter(currentObjetive)) || movements.size() < 1){
		currentObjetive = p.getObetive(getMapCoordinates(this->getCenterPosition()));
		std::vector<sf::Vector2i> ob;
		ob.push_back(currentObjetive);
		pathFinding(getMapCoordinates(getCenterPosition()), ob, movements, me, TypeSeekIA::BEST_PATH, false);
		return true;
	}
	return false;
}

bool PlayerIAEntity::updatePerseguirState(){
	if(!somePlayerEnemyOnRange(getMapCoordinates(getCenterPosition()), getPowerOfBombs(), team)){
		tryKillAEnemy(me, movements, sg._PerseguirStruct.RangoVision, sg._PerseguirStruct.costDestroyWall);
		return true;
	}
	return false;
}

/* Determina estado actual */
void PlayerIAEntity::updateState(){
	if(currentState == StateIA::PATROL){
		
	}
}

void PlayerIAEntity::startStates(){
	if(this->sg.havePatrolStruct){
		currentState = StateIA::PATROL;
		p = sg.getPatrol(getMapCoordinates(getCenterPosition()));
		currentObjetive = p.getObetive(getMapCoordinates(this->getCenterPosition()));
	}else{
		currentState = StateIA::PERSEGUIR;
	}
}


bool PlayerIAEntity::updatePlayer(){
	generatePathStates();
	
	updateMovement();

	updateVelocity();

	// Call animate function to change current sprite if needed.
	animate(velocity);

	// Move Entity position
	if (!expiredEntity) {
		move(velocity.x, velocity.y);
		if (BombTaked != nullptr) {//Si tiene bomba, actualizar a la posicion del jugador (centrado segun cuadricula)
			BombTaked->setPosition(Level::getMapCellCorner(this->getCenterPosition()));
		}
	}
	return false;
}



//Upadate
// -> Upadte State (nuevo estado/continuar estado)
// -> generar camino
// -> generar movimiento