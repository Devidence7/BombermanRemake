#include "../Map/Level.hpp"
#include "../Include/EntitiesInclude.hpp"
#include "../PseudoPPDL/Estados.hpp"
#include "../Utils/IAFunctions.hpp"

//	sf::FloatRect getGlobalBounds() const override; Modificar??

//	void update() override;
void PlayerIAEntity::setCollision(std::shared_ptr<Entity> col){
	this->Entity::setCollision(col);

	
//	std::cout << "Omited areas on collision " << OmittedAreas.size() << "\n";
	

	sf::Vector2i dir(0,0);
	switch (lastMovement)
	{
	case LookingAt::down :
		dir.y = 1;
		break;
	case LookingAt::up :
		dir.y = -1;
		break;
	case LookingAt::left :
		dir.x = -1;
		break;
	case LookingAt::right :
		dir.x = 1;
		break;
	default:
		break;
	}

	dir = dir + getEntityMapCoordinates();
	sf::Vector2i colPos = col->getEntityMapCoordinates();
	if(dir.x != colPos.x || dir.y != colPos.y ){
		//Si no esta intentando llegar a la posicion donde ha colisionado
		//std::cout << "No hay nda que evitar\n";
		return;
	}

	
	movements.clear();
	currentMovement = nullptr;
	if(std::dynamic_pointer_cast<Bomb>(col) != nullptr){
		OmittedAreas.clear();
		generateOmitedZones(getEntityMapCoordinates(), OmittedAreas, sg._PerseguirStruct.RangoVision);
		currentState = RUNAWAY;
		go2SafeZone(getMapCoordinates(getCenterPosition()),me, movements);
	}else{
		std::cout << "DEcidir en Colision\n";
		decildetState();
	}
}

void PlayerIAEntity::setExpiredEntity(){
	PlayerEntity::setExpiredEntity();
	this->currentState = StateIA::NON_OBJETIVE;
	this->currentMovement = nullptr;
	this->movements.clear();
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
	if(this->expiredEntity || this->dead || this->respawning){
		return;
	}
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
	sf::FloatRect body = getGlobalBounds();
	if (velocity.x != 0) {
		if (velocity.x > 0) {
			lastMovement = LookingAt::right;
			if(velocity.x > body.width/2){
				velocity.x = body.width/2;
			}
		}
		else {
			lastMovement = LookingAt::left;
			if(velocity.x < -body.width/2){
				velocity.x = -body.width/2;
			}
		}
	}
	else if (velocity.y != 0) {
		if (velocity.y > 0) {
			lastMovement = LookingAt::down;
			if(velocity.y > body.height/2){
				velocity.y = body.height/2;
			}
		}
		else {
			lastMovement = LookingAt::up;
			if(velocity.y < -body.height/2){
				velocity.y = -body.height/2;
			}
		}
	}
	
	if(StateIA::THROWING_BOMB == currentState){
		lastMovement = objetiveTo;
		velocity = sf::Vector2f(0,0);
	}else if(actionAvaible == ActionsAvalible::REMOTE_BOMB && justActionDone){
		velocity = sf::Vector2f(0,0);
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
void PlayerIAEntity::updateFarm(){
	if(movements.size() < 1 && currentMovement == nullptr){
		putABomb();
	}else if(isObjetiveFarmOnRange()){
	}else{
		if(movements.size() > 2){
			
			std::vector<sf::Vector2i> ob;
			ob.push_back(movements.back()->getPosition());
			std::list<ANode_Ptr> mo;
			if(pathFindingGoWithCare(this->getEntityMapCoordinates(), ob , mo, me, 0)){
				currentMovement = nullptr;
				movements = mo;
			}

		}
	}
}
void PlayerIAEntity::updateCathPU(){
	if((movements.size() < 1 && currentMovement == nullptr) || (movements.size() > 0 && std::dynamic_pointer_cast<PowerUp>(Level::getCellMiniMapObject(movements.back()->getPosition())) == nullptr)){
			currentState = StateIA::NON_OBJETIVE;
	}else{
		if(movements.size() > 2){
			
			std::vector<sf::Vector2i> ob;
			ob.push_back(movements.back()->getPosition());
			std::list<ANode_Ptr> mo;
			if(pathFindingGoWithCare(this->getEntityMapCoordinates(), ob , mo, me, 0)){
				currentMovement = nullptr;
				movements = mo;
			}

		}
	}
}




void PlayerIAEntity::putABomb(){
	//OmittedAreas.push_back(OmittedArea(getEntityMapCoordinates()));
	if(dead || respawning){
		currentState = StateIA::NON_OBJETIVE;
		movements.clear();
		currentMovement = nullptr;
		return;
	}
	OmittedAreas.clear();
	generateOmitedZones(getEntityMapCoordinates(), OmittedAreas, sg._PerseguirStruct.RangoVision);

	if(std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(getEntityMapCoordinates())) != nullptr){
		addThisZone2OmittedArea(getEntityMapCoordinates(), OmittedAreas, powerOfBombs);
		currentState = StateIA::RUNAWAY;
		this->currentMovement = nullptr;
		go2SafeZone(getMapCoordinates(getCenterPosition()),me, movements);
	}else if(haveBombs()  && canPutABombSafe(getMapCoordinates(getCenterPosition()),me, movements) ){
		lastPositionKnowed = getCenterPosition();
		if(movements.size() < 2){
			//std::cout << "Size M. " << movements.size() << "\n";
		}
		if (Level::addBomb(this->me))
		{		
			//std::cout << "put A Bomb\n";
			numOfBombs--;
		}
		currentState = StateIA::RUNAWAY;
		this->currentMovement = nullptr;
	}else if(!haveBombs()){
		Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(BombsAsociated.front());
		if(WillDead(getEntityMapCoordinates(), b->getExplosionTimeLeft())){
			this->currentMovement = nullptr;
			movements.clear();
			//canPutABombSafe(getEntityMapCoordinates(), me, movements);
			go2SafeZone(getEntityMapCoordinates(), me, movements);
			if(movements.size() < 1){
				//Bloqueado
				LookingAt at;
				if(PanicMode(getEntityMapCoordinates(), actionAvaible, at, movements) ){
					std::cout << "Pani mode\n";
					currentMovement = nullptr;
					objetiveTo = at;
					currentState = StateIA::PANIC_MODE;
				}else{
					std::cout << "NO Panic mode\n";
				}
			}else{
				currentState = StateIA::RUNAWAY;
			}
		}
	}else{
		//std::cout<< "Estoy atascado\n";
		//currentState = StateIA::RUNAWAY;
		//this->currentMovement = nullptr;
		//canPutABombSafe(getMapCoordinates(getCenterPosition()),me, movements);
	}
}


bool PlayerIAEntity::updateKillState(){
	//if(somePlayerEnemyOnRange(getMapCoordinates(getCenterPosition()), getPowerOfBombs(), team)){
		movements.clear();
		currentMovement = nullptr;
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
	LookingAt lAt;
	float pointKill = 0;
	float pointAvility = 0;

	float pointFollow = 0;
	float pointFarm = 0;
	float pointGoToPU = 0;
	if(somePlayerEnemyOnRange(getMapCoordinates(getCenterPosition()), getPowerOfBombs(), team) && haveBombs()){
		pointKill = sg._KillStruct.ansiansDeKill * 3;
	}
	if(this->CanGrabBomb()){
		if(somePlayerEnemyOnRangeThrow(getEntityMapCoordinates(), getPowerOfBombs() + 5, team, lAt) && haveBombs()){
			pointAvility =  sg._KillStruct.ansiansDeKill * 3 + 3;
			pointKill = sg._KillStruct.ansiansDeKill * 3;
		}
	}else if(this->CanKickBom()){

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
	if(pointFollow != 0 && (pointFollow < pointFarm || pointFarm == 0) && (pointFollow < pointKill || pointKill==0) && (pointFollow < pointGoToPU || pointGoToPU == 0)){
		movements = movementes2Players;
		this->currentState = StateIA::PERSEGUIR;	
	}else if(pointFarm != 0 && (pointFarm < pointKill || pointKill == 0 ) &&  (pointFarm < pointGoToPU || pointGoToPU == 0)){
		movements = movementes2Farm;
		this->currentState = StateIA::FARM;	
	}else if(pointKill != 0 &&  (pointKill < pointGoToPU || pointGoToPU == 0)){
		this->currentState = StateIA::KILL;	
		if(pointAvility != 0 && haveBombs()){
			this->currentState = StateIA::THROWING_BOMB;
			objetiveTo = lAt;
			//movements.clear();
		}
	}else if(pointGoToPU != 0){
		this->currentState = StateIA::CATCH_PU;	
		movements = movementes2PE;
	}else{
		if(WillDead(getEntityMapCoordinates(), -1)){
			//canPutABombSafe(getEntityMapCoordinates(), me, movements);
			go2SafeZone(getEntityMapCoordinates(), me, movements);
			if(movements.size() < 1){
				//Bloqueado
				LookingAt at;
				if(PanicMode(getEntityMapCoordinates(), actionAvaible, at, movements) ){
					std::cout << "Pani mode\n";
					currentMovement = nullptr;
					objetiveTo = at;
					currentState = StateIA::PANIC_MODE;
				}else{
					std::cout << "NO Panic mode\n";
				}
			}else{
				this->currentState = StateIA::RUNAWAY;				
			}
		}else{
			this->currentState = StateIA::NON_OBJETIVE;	
		}
	}
	
}

void PlayerIAEntity::kickingState(){
	sf::Vector2i kickCell;
		std::cout << "Update Kickin\n";
	
	if(currentState != StateIA::KICKING_BOMB){
		return;
	}
	if(justActionDone){
		
		if((GameTime::getTimeNow() - lastActionDone) > 0.5){
			std::cout << "Fin pateo\n";
			justActionDone = false;
			lastActionDone = GameTime::getTimeNow();
			go2SafeZone(getEntityMapCoordinates(), me, movements);
			if(movements.size() < 1){
				currentState = StateIA::NON_OBJETIVE;
			}else{
				currentState = StateIA::RUNAWAY;
			}
		}
	}else if(actionAvaible != ActionsAvalible::KICK_BOM){
			std::cout << "NO PUEDE PATEAR BOMBAS\n";
			currentMovement = nullptr;
			movements.clear();
			currentState = StateIA::NON_OBJETIVE;
			lastActionDone = GameTime::getTimeNow();
			justActionDone = false;
	}else if(movements.size() < 1 && currentMovement == nullptr){
		std::cout << "Intentando patear\n";
		lastMovement = objetiveTo;
		if(canKickBomb(kickCell)){
			//Shot
			Level::KickBomb(me, kickCell);
			lastActionDone = GameTime::getTimeNow();
			justActionDone = true;
		}
	}else{
		std::cout << "en movimiento\n";
	}
}


void PlayerIAEntity::ThrowingState(){
	movements.clear();
	currentMovement = nullptr;
	sf::Vector2i bombPos;
	if(justActionDone){
		
		if((GameTime::getTimeNow() - lastActionDone) > 0.5){
			justActionDone = false;
			lastActionDone = GameTime::getTimeNow();
			go2SafeZone(getEntityMapCoordinates(), me, movements);
			if(movements.size() < 1){
				currentState = StateIA::NON_OBJETIVE;
			}else{
				currentState = StateIA::RUNAWAY;
			}
		}
	}else if(BombTaked != nullptr){
		if(GameTime::getTimeNow() - lastActionDone > frameSpeed){
			lastMovement = objetiveTo;
			
			currentMovement = nullptr;
			TryThrowBomb();
			movements.clear();
			lastActionDone = GameTime::getTimeNow();
			justActionDone = true;
		}
	}else if(!CanGrabBomb()){
			//std::cout << "NO PUEDE COGER BOMBAS\n";
			currentMovement = nullptr;
			movements.clear();
			currentState = StateIA::NON_OBJETIVE;
			lastActionDone = GameTime::getTimeNow();
			justActionDone = false;
	//}else if(nullptr == Level::getCellMiniMapObject(getEntityMapCoordinates())){
	}else if(!canGrabBombIA(bombPos)){
		Level::addBomb(me);
		//std::cout << "trow Bomb\n";

		if(currentMovement != nullptr){
		//	std::cout << "CurrentM " << currentMovement->getPosition().x << " " << currentMovement->getPosition().y << "\n";
		}
		if(movements.size() > 0){
	//		std::cout << "Size M. " << movements.size() << "\n";
		}
		lastActionDone = GameTime::getTimeNow();
	}else{
		if(GameTime::getTimeNow() - lastActionDone > frameSpeed){
			TryGrabBomb();
			lastActionDone = GameTime::getTimeNow();
		}
	}
}

/* Determina estado actual */
void PlayerIAEntity::updateState(){
	sf::Vector2i currentPosMap = getMapCoordinates(getCenterPosition());
	sf::Vector2i currentMovemPos;
	if(currentMovement != nullptr){
	 	currentMovemPos = currentMovement->getPosition();
	}
	OmittedAreas.clear();
	generateOmitedZones(currentPosMap, OmittedAreas, sg._PerseguirStruct.RangoVision);
	for(Entity_ptr e : BombsAsociated){
		Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(e);
		if(b->onFlight){
			this->OmittedAreas.push_back(OmittedArea(getMapCoordinates(b->getObjetive()) ));
		}
	}
	if(currentState != StateIA::THROWING_BOMB && currentState != StateIA::KICKING_BOMB && currentState != PANIC_MODE  &&  movements.size() < 1
				&& (currentMovement == nullptr || currentMovemPos.x == currentPosMap.x && currentMovemPos.y == currentMovemPos.y)  
				&& WillDead(currentPosMap, -1)){
		currentMovement = nullptr;
		movements.clear();
		currentState = StateIA::NON_OBJETIVE;
	}else if(currentState != StateIA::THROWING_BOMB && currentState != PANIC_MODE && currentState != StateIA::KICKING_BOMB && actionAvaible != ActionsAvalible::THROUGH_BOMB && currentState != StateIA::RUNAWAY && std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(currentPosMap)) != nullptr ){
		currentMovement = nullptr;
		movements.clear();
		currentState = StateIA::NON_OBJETIVE;
	}
	switch (currentState)
	{
	case StateIA::NON_OBJETIVE:
			decildetState();
			break;
	case StateIA::PERSEGUIR:
		if(somePlayerEnemyOnRange(currentPosMap, getPowerOfBombs(), team)){
			currentState =  StateIA::KILL;
		}
		LookingAt at;
		if(this->CanGrabBomb() && somePlayerEnemyOnRangeThrow(currentPosMap, getPowerOfBombs(), team, at)){
			currentState = StateIA::THROWING_BOMB;
			objetiveTo = at;
			//movements.clear();
		}else if(actionAvaible == ActionsAvalible::KICK_BOM && canArraiveBombKickin()){
			currentMovement = nullptr;
			currentState = StateIA::KICKING_BOMB;
			Level::addBomb(me);
		}
	case  StateIA::FARM:
		updateFarm();
		break;
	case StateIA::KILL:
	break;
	case StateIA::THROWING_BOMB:
		ThrowingState();
		break;
	case StateIA::KICKING_BOMB:
		kickingState();
		break;
	case StateIA::CATCH_PU:
		updateCathPU();
		break;
	case StateIA::PANIC_MODE:
	updatePanicMode();
	break;
	default:
		break;
	}
	if(StateIA::NON_OBJETIVE){
		movements.clear();
		currentMovement = nullptr;
		std::cout << "NON OBJ\n";
		if(pathFindingGoSafeArea(getEntityMapCoordinates(), movements, me, 0)){
			if(movements.size() < 1){
				LookingAt at;
				if(PanicMode(getEntityMapCoordinates(), actionAvaible, at, movements) ){
					std::cout << "Pani mode\n";
					currentMovement = nullptr;
					objetiveTo = at;
					currentState = StateIA::PANIC_MODE;
				}else{
					std::cout << "NO Panic mode\n";
				}
			}else{
				currentState = StateIA::RUNAWAY;
			}
		}
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
	if(this->expiredEntity || this->dead || this->respawning){
		return;
	}
	if(actionAvaible == ActionsAvalible::REMOTE_BOMB && justActionDone && GameTime::getTimeNow() - lastActionDone > 0.2){
		justActionDone = false;
	}
	if(this->actionAvaible == ActionsAvalible::REMOTE_BOMB && this->BombsAsociated.size() > 0){
		if(!isOnRangeExplosion(BombsAsociated.front()->getEntityMapCoordinates(), getCenterPosition(), getGlobalBounds() ,getPowerOfBombs())){
			BombsAsociated.front()->setExpiredEntity();
			justActionDone = true;
			lastActionDone = GameTime::getTimeNow();
		}
	}
}
bool PlayerIAEntity::updatePlayer(){
	if(this->dead){
		return false;
	}
	//std::cout << "c State " << State2string(currentState) <<"\n";

	if(!this->expiredEntity && !this->dead && !this->respawning){
		checkExploteRemote();
		generatePathStates();
		updateMovement();
	}
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


bool PlayerIAEntity::canKickBomb(sf::Vector2i &lookinCell){
	if(actionAvaible != ActionsAvalible::KICK_BOM){
		return false;
	}
	lookinCell = getEntityMapCoordinates();
	switch (lastMovement)
	{
	case LookingAt::down :
		lookinCell.y++;
		break;
	case LookingAt::up :
		lookinCell.y--;
		break;
	case LookingAt::left :
		lookinCell.x--;
		break;
	case LookingAt::right :
		lookinCell.x++;
		break;

	default:
		break;
	}
	if(Level::isValidCell(lookinCell)){
		Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(lookinCell));
		return b != nullptr;// && canKickBombSafe(lookinCell, b->bombPower, lastMovement);
	}
	return false;
}


bool PlayerIAEntity::canGrabBombIA(sf::Vector2i &lookinCell){
	if(actionAvaible != ActionsAvalible::GRAB_BOMB){
		return false;
	}	
	lookinCell = getEntityMapCoordinates();
	Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(lookinCell));
	if(b == nullptr){
		switch (lastMovement)
		{
		case LookingAt::down :
			lookinCell.y++;
			break;
		case LookingAt::up :
			lookinCell.y--;
			break;
		case LookingAt::left :
			lookinCell.x--;
			break;
		case LookingAt::right :
			lookinCell.x++;
			break;

		default:
			break;
		}
		if(Level::isValidCell(lookinCell)){
			b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(lookinCell));
		}
	}
	return b != nullptr;
}

bool PlayerIAEntity::canThrowBomb(){
	return this->BombTaked != nullptr && canThrowBombSafe(getEntityMapCoordinates(), std::dynamic_pointer_cast<Bomb>(BombTaked)->bombPower, this->lastMovement);
}


bool PlayerIAEntity::TryGrabBomb(){
	sf::Vector2i bombPosition;
	if(BombTaked != nullptr || !canGrabBombIA(bombPosition)){
		return false;
	}
	return Level::TakeBomb(me, bombPosition);
}

bool PlayerIAEntity::TryKickBomb(){
	sf::Vector2i bombPosition;
	if(!canKickBomb(bombPosition)){
		return false;
	}
	return Level::KickBomb(me, bombPosition);
}

bool PlayerIAEntity::TryThrowBomb(){
	if(BombTaked == nullptr){
		return false;
	}
	Level::ThrowBomb(me, std::dynamic_pointer_cast<Bomb>(BombTaked));
	BombTaked.reset();
	bombThrowed = true;
	lastThrowedTime = GameTime::getTimeNow();
	currentFrame = 0;
	return true;
}
bool PlayerIAEntity::isObjetiveOnRange(){
	if(movements.size() < 1 || !haveBombs()  || actionAvaible != ActionsAvalible::GRAB_BOMB){
		return false;
	}
	ANode_Ptr obj = movements.back();
	sf::Vector2i objPosition = obj->getPosition();
	sf::Vector2i positionIA = getEntityMapCoordinates();
	sf::Vector2i sLevel = Level::sizeLevel();
	bool OnRange = false;
	if(abs(min(positionIA.x + 5, sLevel.x) - positionIA.x) > 1 && isOnRangeBomb(sf::Vector2i(min(positionIA.x + 5, sLevel.x) , positionIA.y), objPosition, getPowerOfBombs())){
		//lanzar hacia dch
		OnRange = true;
		objetiveTo = LookingAt::right;
	}
	if(abs(max(positionIA.x - 5, 1) - positionIA.x) > 1  && isOnRangeBomb(sf::Vector2i(max(positionIA.x - 5, 1) , positionIA.y), objPosition, getPowerOfBombs())){
		//lanzar hacia izq
		OnRange = true;
		objetiveTo = LookingAt::left;
	}
	
	if(abs(min(positionIA.y + 5, sLevel.y) - positionIA.y) > 1  && isOnRangeBomb(sf::Vector2i(positionIA.x, min(positionIA.y + 5, sLevel.y) ), objPosition, getPowerOfBombs())){
		//lanzar abajo
		OnRange = true;
		objetiveTo = LookingAt::down;
	}
	if(abs(max(positionIA.y - 5, 1) - positionIA.y) > 1  && isOnRangeBomb(sf::Vector2i(positionIA.x, max(positionIA.y - 5, 1) ), objPosition, getPowerOfBombs())){
		//lanzar arriba
		OnRange = true;
		objetiveTo = LookingAt::up;
	}
	return OnRange;
}

bool PlayerIAEntity::objetiveOnExactlyThrowBomb(){
	if(movements.size() < 1 || !haveBombs()  || actionAvaible != ActionsAvalible::GRAB_BOMB){
		return false;
	}
	ANode_Ptr obj = movements.back();
	sf::Vector2i objPosition = obj->getPosition();
	sf::Vector2i positionIA = getEntityMapCoordinates();
	sf::Vector2i sLevel = Level::sizeLevel();
	bool OnRange = false;
	sf::Vector2i fallPossR(min(positionIA.x + 5, sLevel.x-2) , positionIA.y);
	sf::Vector2i fallPossL(max(positionIA.x - 5, 1) , positionIA.y);
	sf::Vector2i fallPossU(positionIA.x, max(positionIA.y - 5, 1));
	sf::Vector2i fallPossD(positionIA.x, min(positionIA.y + 5, sLevel.y - 2));

	if(abs(fallPossR.x - positionIA.x) > 1  && fallPossR.x == objPosition.x && fallPossR.y == objPosition.y ){
		//lanzar hacia dch
		OnRange = true;
		objetiveTo = LookingAt::right;
	}else if(abs(fallPossL.x - positionIA.x) > 1 && fallPossL.x == objPosition.x && fallPossL.y == objPosition.y){
		//lanzar hacia izq
		OnRange = true;
		objetiveTo = LookingAt::left;
	}else if(abs(fallPossD.y - positionIA.y) > 1  && fallPossD.x == objPosition.x && fallPossD.y == objPosition.y){
		//lanzar abajo
		OnRange = true;
		objetiveTo = LookingAt::down;
	}else if(abs(fallPossU.x - positionIA.y) > 1  && fallPossU.x == objPosition.x && fallPossU.y == objPosition.y){
		//lanzar arriba
		OnRange = true;
		objetiveTo = LookingAt::up;
	}
	
	return OnRange;
}

bool PlayerIAEntity::useAvility(){
	bool used = false;
	if(BombTaked != nullptr){
		return TryThrowBomb();
	}
	switch (actionAvaible)
	{
	case ActionsAvalible::GRAB_BOMB:
		used = TryGrabBomb();
		break;
	case ActionsAvalible::KICK_BOM:
		used = TryKickBomb();
	default:
		break;
	}
	return used;
}

bool PlayerIAEntity::isObjetiveFarmOnRange(){
	if(actionAvaible == ActionsAvalible::GRAB_BOMB){
		if(objetiveOnExactlyThrowBomb()){
			movements.clear();
			currentMovement = nullptr;
			currentState = StateIA::THROWING_BOMB;
			return true;
		}
	}else if(actionAvaible == ActionsAvalible::KICK_BOM) {
		if(canArraiveBombKickinExactly()){
			currentMovement = nullptr;
			currentState = StateIA::KICKING_BOMB;
			Level::addBomb(me);
			return true;
		}
	}
	return false;
}

bool PlayerIAEntity::canArraiveBombKickin(){
	if(movements.size() < 2 || !haveBombs()  || actionAvaible != ActionsAvalible::KICK_BOM){
		return false;
	}

	sf::Vector2i objPosition = movements.back()->getPosition();
	sf::Vector2i cPosition = getEntityMapCoordinates();

	if(objPosition.x != cPosition.x && objPosition.y != cPosition.y){
		return false;
	}
	if(objPosition.x == cPosition.x && objPosition.y == cPosition.y){
		return false;
	}
	if(checkIsDirectAccesible(cPosition, objPosition)){
		sf::Vector2f Dir = normalize(objPosition - cPosition);
		sf::Vector2i iDir(Dir.x, Dir.y);
		
		Entity_ptr e = Level::getCellMiniMapObject(cPosition - iDir);
		bool space2kick = e == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr;
		if(space2kick){
			if(Dir.x < 0){
				objetiveTo = LookingAt::left;
			}else if(Dir.x > 0){
				objetiveTo = LookingAt::right;
			}
			else if(Dir.y > 0){
				objetiveTo = LookingAt::down;
			}
			else if(Dir.y < 0){
				objetiveTo = LookingAt::up;
			}
			movements.clear();
			movements.push_back(std::make_shared<ANode>(ANode(sf::Vector2i(cPosition - iDir), 0,0)));
			//movements.push_back(std::make_shared<ANode>(sf::Vector2i(cPosition)));
		}
		return space2kick;
	}
	return false;
}

bool PlayerIAEntity::canArraiveBombKickinExactly(){
	if(movements.size() < 2 || !haveBombs()  || actionAvaible != ActionsAvalible::KICK_BOM){
		return false;
	}

	sf::Vector2i objPosition = movements.back()->getPosition();
	sf::Vector2i cPosition = getEntityMapCoordinates();

	if(objPosition.x != cPosition.x && objPosition.y != cPosition.y){
		return false;
	}
	if(objPosition.x == cPosition.x && objPosition.y == cPosition.y){
		return false;
	}
	if(checkIsDirectAccesible(cPosition, objPosition)){
		sf::Vector2f Dir = normalize(objPosition - cPosition);
		sf::Vector2i iDir(Dir.x, Dir.y);
		if(Level::isValidCell(objPosition + iDir)){
			Entity_ptr nextObjetive = Level::getCellMiniMapObject(objPosition + iDir);
			if(nextObjetive == nullptr || std::dynamic_pointer_cast<PowerUp>(nextObjetive) != nullptr){
				//No parara en objetivo
				return false;
			}
		}
		Entity_ptr e = Level::getCellMiniMapObject(cPosition - iDir);
		bool space2kick = e == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr;
		if(space2kick){
			if(Dir.x < 0){
				objetiveTo = LookingAt::left;
			}else if(Dir.x > 0){
				objetiveTo = LookingAt::right;
			}
			else if(Dir.y > 0){
				objetiveTo = LookingAt::down;
			}
			else if(Dir.y < 0){
				objetiveTo = LookingAt::up;
			}
			movements.clear();
			movements.push_back(std::make_shared<ANode>(ANode(sf::Vector2i(cPosition - iDir), 0,0)));
			//movements.push_back(std::make_shared<ANode>(sf::Vector2i(cPosition)));
		}
		return space2kick;
	}
	return false;
}

//Upadate
// -> Upadte State (nuevo estado/continuar estado)
// -> generar camino
// -> generar movimiento


void PlayerIAEntity::updatePanicMode(){
	lastMovement = objetiveTo;
	switch (actionAvaible)
	{
	case ActionsAvalible::GRAB_BOMB :
		ThrowingState();
	break;
	case ActionsAvalible::KICK_BOM :
		kickingState();
		break;
	default:
		break;
	}
}