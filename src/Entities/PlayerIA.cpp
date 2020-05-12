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
	
	if(std::dynamic_pointer_cast<Bomb>(col) != nullptr){
		currentState = RUNAWAY;
		canPutABombSafe(getMapCoordinates(getCenterPosition()),me, movements);
	}else{
		movements.clear();
		currentMovement = nullptr;
		decildetState();
	}
	
}

//	void realizeActions();

bool PlayerIAEntity::playerActions(){
	return false;
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
	updateState();
	bool generatedNewObj = false;
	switch (currentState)
	{
	case StateIA::PATROL:
		generatedNewObj = updatePatrolState();
		break;
	case StateIA::PERSEGUIR:
		generatedNewObj = updatePerseguirState();
		break;
	case StateIA::KILL:
		updateKillState();
		break;
	case StateIA::RUNAWAY:
		updateRunawayState();
	default:
		break;
	}
}

void PlayerIAEntity::updateMovement(){

	if(currentMovement != nullptr &&  checkArrivePosition(this->getCenterPosition(), currentMovement->getPosition(), currentMovement->getAction())){
		currentMovement = nullptr;
	}
	
	if(currentMovement == nullptr && movements.size() < 1){
		//currentState = StateIA::NON_OBJETIVE;
		//currentMovement = std::make_shared<ANode>(ANode(getMapCoordinates(getCenterPosition()), sf::Vector2i(0,0), getMapCoordinates(getCenterPosition()), 0));
	}
	else if (currentMovement == nullptr)
	{ //Si esta en posicions
		currentMovement = movements.front();
		movements.pop_front();
		numMovenet --;
	}
}

bool PlayerIAEntity::updateVelocity(){
	velocity.x = 0;
	velocity.y = 0;
	double moveTime = 0;
	if (lastMovementTime) {
		moveTime = (GameTime::getTimeNow() - lastMovementTime) * 60;
	}
	lastMovementTime = GameTime::getTimeNow();
	if(currentMovement == nullptr){
		return false;
	}
	sf::Vector2f nextPosition = MapCoordinates2GlobalCoorCenter(currentMovement->getPosition());
	sf::Vector2f currentPosition = getCenterPosition();
	
	sf::Vector2f dir = nextPosition - currentPosition;
	velocity = normalize(dir);
	
	if(abs(dir.x) < 1 && abs(dir.y) < 1){
		velocity.x = 0;		
		velocity.y = 0;		

	}else if(abs(velocity.x) < 0.1){
		velocity.x = 0;
		if(velocity.y > 0){
			velocity.y = 1;
		}else{
			velocity.y = -1;
		}
		//velocity.y = velocty.y/abs(velocity.y);
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

bool PlayerIAEntity::updateRunawayState(){
	if(movements.size()< 1 && currentMovement == nullptr){
		currentState = StateIA::NON_OBJETIVE;
	}
	return false;
}

void PlayerIAEntity::putABomb(){
	//OmittedAreas.push_back(OmittedArea(getEntityMapCoordinates()));
	if(haveBombs() && canPutABombSafe(getMapCoordinates(getCenterPosition()),me, movements) ){
		lastPositionKnowed = getCenterPosition();
		if (Level::addBomb(this->me))
		{		
			numOfBombs--;
			currentState = StateIA::RUNAWAY;
			this->currentMovement = nullptr;
		}
		if(moduleVector(getCenterPosition() - lastPositionKnowed) > 20){
			std::cout << "Al poner bomba, a tomar por culo\n";
		}
	}else if(haveBombs()){
		std::cout << "IS not safe or not Bobms\n";
	}
}


bool PlayerIAEntity::updateKillState(){
	//if(somePlayerEnemyOnRange(getMapCoordinates(getCenterPosition()), getPowerOfBombs(), team)){
		movements.clear();
		putABomb();
	//	return true;
	//}
	return false;
}

void PlayerIAEntity::decildetState(){
	std::list<ANode_Ptr> movementes2Players;
	std::list<ANode_Ptr> movementes2Farm;
	std::list<ANode_Ptr> movementes2PE;
	std::vector<sf::Vector2i> objetivePlayers;
	std::vector<sf::Vector2i> objetivesPE;
	float pointKill = 0;
	float pointFollow = 0;
	float pointFarm = 0;
	float pointGoToPU = 0;
	if(somePlayerEnemyOnRange(getMapCoordinates(getCenterPosition()), getPowerOfBombs(), team)){
		pointKill = sg._KillStruct.ansiansDeKill * 3;
	}
	selectEnemyPlayers(me, objetivePlayers, this->sg._PerseguirStruct.RangoVision);
	if(objetivePlayers.size() > 0){
		if(pathFindingGoWithCare(this->getEntityMapCoordinates(), objetivePlayers , movementes2Players, me, 0)){
			pointFollow = 1/movementes2Players.back()->costNode() * sg._KillStruct.ansiansDeKill;
		}
	}
		
	if(pathFinderDestroy2Farm(this->getEntityMapCoordinates(), movementes2Farm, me, 0)){
		pointFarm = 1/movementes2Farm.back()->costNode() * getIntersetDestroyWalls();
	}
	selectPowerUpsObjetive(me, objetivesPE, this->sg._SeekPEStruct.RangoVision);
	if(objetivesPE.size() > 0){
		if(pathFindingGoWithCare(this->getEntityMapCoordinates(), objetivesPE , movementes2PE, me, 0)){
			pointGoToPU = 1/movementes2PE.back()->costNode() * sg._SeekPEStruct.bootsSeek;
		}
	}

	//std::cout << "Points K " << pointKill << " - Farm "  << pointFarm << " - GPU " << pointGoToPU << " - Follow " << pointFollow  <<"\n"; 
	if(pointFollow > 0 && (pointFollow < pointFarm || pointFarm == 0) && (pointFollow < pointKill || pointKill==0) && (pointFollow < pointGoToPU || pointGoToPU == 0)){
		movements = movementes2Players;
		this->currentState = StateIA::PERSEGUIR;	
	}else if(pointFarm > 0 && (pointFarm < pointKill || pointKill == 0 ) &&  (pointFarm < pointGoToPU || pointGoToPU == 0)){
		movements = movementes2Farm;
		this->currentState = StateIA::FARM;	
	}else if(pointKill > 0 &&  (pointKill < pointGoToPU || pointGoToPU == 0)){
		this->currentState = StateIA::KILL;	
	}else{
		this->currentState = StateIA::CATCH_PU;	
		movements = movementes2PE;
	}
	
}

/* Determina estado actual */
void PlayerIAEntity::updateState(){
	sf::Vector2i currentPosMap = getMapCoordinates(getCenterPosition());
	OmittedAreas.clear();
	generateOmitedZones(currentPosMap, OmittedAreas, sg._PerseguirStruct.RangoVision);
	
	switch (currentState)
	{
	case StateIA::NON_OBJETIVE:
		//Si tiene en rango a algun jugador
		/* if(sg.havePatrolStruct){
			currentState = StateIA::PATROL;
		}else   */if(somePlayerEnemyOnRange(currentPosMap, getPowerOfBombs(), team)){
			currentState =  StateIA::KILL;
		}else {
			decildetState();
		}

		break;
	case StateIA::PERSEGUIR:
		if(somePlayerEnemyOnRange(currentPosMap, getPowerOfBombs(), team)){
				currentState =  StateIA::KILL;
		}
	case  StateIA::FARM:
		if(movements.size() < 1 && currentMovement == nullptr){
			putABomb();
		}
		break;
	case StateIA::CATCH_PU:
		if((movements.size() < 1 && currentMovement == nullptr) || (movements.size() > 1 && std::dynamic_pointer_cast<PowerUp>(Level::getCellMiniMapObject(movements.back()->getPosition())) != nullptr)){
			currentState = StateIA::NON_OBJETIVE;
		}
		break;
	default:
		break;
	}
}

void PlayerIAEntity::startStates(){
	// if(this->sg.havePatrolStruct){
	// 	currentState = StateIA::PATROL;
	// 	p = sg.getPatrol(getMapCoordinates(getCenterPosition()));
	// 	currentObjetive = p.getObetive(getMapCoordinates(this->getCenterPosition()));
	// }else{
		currentState = StateIA::NON_OBJETIVE;
	// }
}

void PlayerIAEntity::checkExploteRemote(){
	if(this->actionAvaible == ActionsAvalible::REMOTE_BOMB && this->BombsAsociated.size() > 0){
		if(!isOnRangeExplosion(BombsAsociated.front()->getEntityMapCoordinates(), getCenterPosition(), getGlobalBounds() ,getPowerOfBombs())){
			BombsAsociated.front()->setExpiredEntity();
		}
	}
}
bool PlayerIAEntity::updatePlayer(){

	checkExploteRemote();
	generatePathStates();
	
	updateMovement();
	updateVelocity();
	// Call animate function to change current sprite if needed.
	animate(velocity);
	//cout<<"velocity Enemigo: "<<velocity.x<<" "<<velocity.y<<endl;
	// Move Entity position
	if (!expiredEntity) {
		move(velocity.x, velocity.y);
		lastPositionKnowed = getCenterPosition();
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
