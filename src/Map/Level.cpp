#include "Level.hpp"
#include "../Music/GameMusic.h"
#include "../Music/GameSounds.h"

std::vector<Entity_ptr> Level::entities;
std::vector<Bomb_ptr> Level::onFlightBombs;
std::vector<std::vector<Entity_ptr>> Level::miniMap;
sf::RectangleShape Level::flooro;
bool Level::exitHasApeared = false;
bool Level::canFinishLevel = false;
bool Level::levelFinished = false;
int Level::numWalls = 0;
int Level::stage = 1;
int Level::numEnemiesLeft = 0;
Teleporter_ptr Level::teleporter = nullptr;
GameOptions* Level::gameOptions;


Level::Level(int dimX, int dimY, bool debug, int stage, GameOptions *gameOptions) {
	Level::exitHasApeared = false;
	Level::canFinishLevel = false;
	Level::levelFinished = false;
	Level::numWalls = 0;
	Level::teleporter = nullptr;
	Level::gameOptions = gameOptions;

	Level::stage = stage;

	// Reserve space for faster insert, delete of the entities
	entities.reserve(10000);
	// Create map matrix:
	EntityMap::entityMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
	miniMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
	// Background:
	flooro.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));

	switch (stage) {
	case 1:
		GameMusic::playWorld1Music();
		flooro.setFillColor(sf::Color(0, 102, 10));
		break;
	case 2:
		GameMusic::playWorld2Music();
		flooro.setFillColor(sf::Color(34, 0, 102));
		break;
	case 3:
		GameMusic::playWorld3Music();
		flooro.setFillColor(sf::Color(102, 0, 128));
		break;
	default:
		Level::stage = 3;
		break;
	}



	// Create all pillars:
	for (int x = 0; x < dimX + 2; x++) {
		addPillar(x, 0);
		addPillar(x, dimY + 1);
	}

	for (int y = 0; y < dimY + 2; y++) {
		addPillar(0, y);
		addPillar(dimX + 1, y);
	}

	//create Pillars
	for (int x = 1; x < dimX + 1; x++) {
		for (int y = 1; y < dimY + 1; y++) {
			if (x % 2 == 1 || y % 2 == 1) {
				// Create random Bricks:
				if (!Random::getIntNumberBetween(0, 3)) {
					bool intersec = false;
					for (Enemy_ptr e : Enemies::getVectorEnemies()) {
						sf::Vector2i p = getMapCoordinates(e->getCenterPosition());
						if (p.x == x && p.y == y) {
							intersec = true;
							break;
						}
					}
					if (!intersec && !debug) {
						addWall(x, y);

					}
				}
			}
			else {
				addPillar(x, y);
			}
		}
	}
}

sf::Vector2i Level::sizeLevel() {
	return sf::Vector2i(miniMap[0].size(), miniMap.size());
}

bool Level::isValidCell(sf::Vector2i v) {
	return v.x > 0 && v.y > -1 && miniMap.size() - 1 > v.y && miniMap[0].size() - 1 > v.x;
}

void Level::chechAndFixBombCollision(Bomb_ptr b) {
	sf::FloatRect body = b->getGlobalBounds();
	sf::Vector2f v = normalize(b->getVelocity());
	sf::Vector2f add2Pos(body.height / 2 * v.x, body.height / 2 * v.y);
	sf::Vector2f globalPos = b->getCenterPosition();
	sf::Vector2i mapPosition = getMapCoordinates(globalPos);

	//Eliminar bomba de mapa
	//Curent position
	if (getCellMiniMapObject(mapPosition) == b) {
		getCellMiniMapObject(mapPosition) = nullptr;
	}
	//Verificar 2 coordendas
	sf::Vector2i mapPos2 = sf::Vector2i(mapPosition.x + v.x, mapPosition.y + v.y);
	if (Level::isValidCell(mapPos2) && getCellMiniMapObject(mapPos2) == b) {
		getCellMiniMapObject(mapPos2).reset();
	}
	mapPos2 = sf::Vector2i(mapPosition.x - v.x, mapPosition.y - v.y);
	if (Level::isValidCell(mapPos2) && getCellMiniMapObject(mapPos2) == b) {
		getCellMiniMapObject(mapPos2).reset();
	}

	mapPos2 = getMapCoordinates(globalPos + add2Pos);
	b->canExplote = mapPos2 == mapPosition;
	if (!b->canExplote) {
		bool onColision = true;
		//Mirar colision con mapa
		if (Level::isValidCell(mapPos2)) { //No hace falta mirar si esta en el centro de la casilla
			onColision = false;
			if (getCellMiniMapObject(mapPos2) != nullptr) {
				onColision = (std::dynamic_pointer_cast<PowerUp>(getCellMiniMapObject(mapPos2)) == nullptr);
			}
			//TODO:: Si se añaden mas vectores, añadir aqui!!!!
			onColision = (onColision || PLayers::cehckSomeCollision(b));
			onColision = (onColision || Enemies::cehckSomeCollision(b));
		}
		if (!onColision) {
			getCellMiniMapObject(mapPos2) = b;
		}
		else {
			b->setCollision(nullptr);
			b->onMove = false;
			b->canExplote = true;
			b->setPosition(getMapCellCorner(globalPos));
		}
	}
	getCellMiniMapObject(mapPosition) = b; //Volver a poner bomb
}

void Level::createTeleporter(Entity_ptr it) {
	Level::exitHasApeared = true;
	Teleporter_ptr newObject = std::make_shared<Teleporter>(Teleporter((it)->getPosition()));
	addEntityToMiniMap(newObject, getMapCoordinates((it)->getPosition()));
	addNewItem(newObject);
	GameSounds::teleportAppear();

	teleporter = newObject;
	if (canFinishLevel) {
		teleporter->openTeleporter();
	}
}

void Level::brickWallOutcomes(Entity_ptr it) {

	// Last oportunity to get a teleporter
	if (gameOptions->historyMode && !Level::exitHasApeared && Level::numWalls == 1) {
		createTeleporter(it);
	}
	// Russian Roullete
	else {
		float probability = Random::getFloatNumberBetween(0, 1);
		//cout << probability << endl;
		if (!gameOptions->historyMode) probability /= 2;

		if (probability < 0.30) {
			probability = Random::getFloatNumberBetween(0, 92);
			Entity_ptr newObject = nullptr;
			bool tryTeleport = false;

			if (probability < 10) {
				newObject = std::make_shared<MoreFirePowerUp>(MoreFirePowerUp((it)->getPosition()));
			}
			else if (probability < 20) {
				newObject = std::make_shared<LessFirePowerUp>(LessFirePowerUp((it)->getPosition()));
			}
			else if (probability < 30) {
				newObject = std::make_shared<MoreBombsPowerUp>(MoreBombsPowerUp((it)->getPosition()));
			}
			else if (probability < 40) {
				newObject = std::make_shared<MoreSpeedPowerUp>(MoreSpeedPowerUp((it)->getPosition()));
			}
			else if (probability < 50) {
				newObject = std::make_shared<LessSpeedPowerUp>(LessSpeedPowerUp((it)->getPosition()));
			}
			else if (gameOptions->historyMode && probability < 55) {
				newObject = std::make_shared<MoreTimePowerUp>(MoreTimePowerUp((it)->getPosition()));
			}
			else if (probability < 60) {
				newObject = std::make_shared<GrabBombsPowerUp>(GrabBombsPowerUp((it)->getPosition()));
			}
			else if (probability < 65) {
				newObject = std::make_shared<KickBombsPowerUp>(KickBombsPowerUp((it)->getPosition()));
			}
			else if (probability < 75) {
				newObject = std::make_shared<DisseasePowerUp>(DisseasePowerUp((it)->getPosition()));
			}
			else if (probability < 80) {
				newObject = std::make_shared<PassBombsPowerUp>(PassBombsPowerUp((it)->getPosition()));
			}
			else if (probability < 85) {
				newObject = std::make_shared<RemoteBombPowerUp>(RemoteBombPowerUp((it)->getPosition()));
			}
			else if (gameOptions->historyMode && probability < 87) {
				newObject = std::make_shared<ExtraLifePowerUp>(ExtraLifePowerUp((it)->getPosition()));
			}
			else if (gameOptions->historyMode && !Level::exitHasApeared && probability < 92) {
				createTeleporter(it);
				tryTeleport = true;
			}

			if (newObject != nullptr) {
				addEntityToMiniMap(newObject, getMapCoordinates((it)->getPosition()));
				addNewItem(newObject);
			}
			else if (!tryTeleport) {
				// Do this always?
				getCellMiniMapObject(getMapCoordinates((it)->getPosition())).reset();
				getCellObject(getMapCoordinates((it)->getPosition())).reset();
			}

		}
		else {
			// Do this always?
			getCellMiniMapObject(getMapCoordinates((it)->getPosition())).reset();
			getCellObject(getMapCoordinates((it)->getPosition())).reset();
		}
	}
}

void Level::update() {
	auto it = entities.begin();
	int counter = 0;
	// This is made this way because we need to erase element from a vector while we are iterating
	//std::cout << "Entreando a update\n";
	while (it != entities.end()) {
		sf::FloatRect beginRect = (*it)->getGlobalBoundsCollisioner();
		sf::Vector2f begintPos = (*it)->getPosition();
		// Update the entities.
		(*it)->update();
		if ((*it)->getExpiredEntity()) {
			// If it is a bomb
			std::shared_ptr<Bomb> b;
			std::shared_ptr<BrickWall> brickWall;
			if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr) {
				// Create fires
				createFires(*b);
				// When adding entities vector iterator can be invalidated:
				it = entities.begin() + counter;
			}
			else if ((brickWall = std::dynamic_pointer_cast<BrickWall>((*it))) != nullptr) {
				if (brickWall->isDestroyed) {
					brickWallOutcomes(*it);
				}
				
				
				Level::numWalls -= 1;
			}
			else {
				// Do this always?
				getCellMiniMapObject(getMapCoordinates((*it)->getPosition())).reset();
				getCellObject(getMapCoordinates((*it)->getPosition())).reset();
			}

			// Remove the entity from the list of entities if it expired.

			it->reset();
			it = entities.erase(it);
		}
		else {
			std::shared_ptr<Bomb> b;
			if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr) {

				sf::Vector2f positionBomb = b->getPosition();
				sf::Vector2i positionMap = getMapCoordinates(positionBomb);

				if (b->rePutBomb) {
					Entity_ptr e;
					if ((e = getCellMiniMapObject(positionMap)) == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr) {
						b->rePutBomb = false;
						addEntityToMiniMap((*it), positionMap);
					}
					else {
						b->onFlight = true;
						sf::Vector2f v = normalize(b->getVelocity());
						sf::Vector2i vi = sf::Vector2i(v.x, v.y);
						sf::Vector2i newPos = positionMap + vi;
						if (newPos.x < 1 || newPos.y < 1 || (miniMap.size() - 2) < newPos.y || (miniMap[0].size() - 2) < newPos.x) { //Si se sale del mapa, rebotar
							vi = -vi;
							v = -v;
							b->setVelocity(v);
						}

						b->setObjetive(MapCoordinates2GlobalCoorCorner(positionMap + vi));
					}
				}
				else if (b->onMove) {
					chechAndFixBombCollision(b);
				}
			}
			it++;
			counter++;
		}
	}

	if (teleporter != nullptr) {
		if (Enemies::getVectorEnemies().size() < 1) {
			enemiesDefeated();
		}
		else {
			newEnemiesAppear();
		}

		if (teleporter->playerCross) {
			levelFinished = true;
		}
	}

}

/*
	 * This is a DEBUG method, draws in the RenderWindow the hitbox of the Entity
	*/

void Level::draw(sf::RenderWindow& w) {
	// Draw the flooro:
	w.draw(flooro);

	// Draw the entities
	for (std::vector<Entity_ptr>& v : miniMap) {
		for (Entity_ptr e : v) {
			if (e != nullptr) {
				w.draw(*e);
#ifdef HITBOX_DEBUG_MODE
				e->drawEntityHitbox(w);
#endif
			}
		}
	}

	auto it = onFlightBombs.begin();
	// This is made this way because we need to erase element from a vector while we are iterating
	while (it != onFlightBombs.end()) {
		if (!(*it)->onFlight) {
			// Remove the bomb from the list onFlightBombs if it at floor.
			it->reset();
			it = onFlightBombs.erase(it);
		}
		else {
			w.draw(*(*it));
			++it;
		}
	}
}

bool Level::createFire(int type, int posX, int posY, Player_ptr p) {
	// Get the object in cell
	Entity_ptr e = getCellMiniMapObject(getMapCoordinates(posX, posY));

	if (std::dynamic_pointer_cast<Teleporter>(e) != nullptr) {
		Enemies::insertarEnemigosExtraTeleport(e->getPosition(), 3);
	}

	if (e && e->getIsFireDestroyable() && !e->getFireCanGoThroght()) {
		e->setExpiredEntity();
		addEntity(e);
		//TODO Asignar puntaciones
		if (std::dynamic_pointer_cast<EnemyEntity>(e) != nullptr) {
			p->incrementScore(100);
		}
		else if (std::dynamic_pointer_cast<Player_ptr>(e) != nullptr) {
			p->incrementScore(100);
		}
		p->incrementScore(50);
	}

	if (!e || e->getFireCanGoThroght()) {
		e = nullptr;
		Entity_ptr f = std::make_shared<Fire>(Fire(p, type));
		f->setPosition(posX, posY);
		addNewItem(f);
		//addEntity(f);
		//addEntityToMiniMap(f, posX, posY);
		return false;
	}
	return true;
}

void Level::createFires(Bomb& b) {
	sf::Vector2f pos = b.getPosition();
	Player_ptr p2score = b.player2Score;
	// Central:
	createFire(0, pos.x, pos.y, p2score);
	// Down:
	for (int i = 1; i <= b.bombPower; i++) {
		int fireType = i == b.bombPower ? 2 : 1;
		if (createFire(fireType, pos.x, pos.y + 48 * i, p2score)) {
			break;
		}
	}
	// Up:
	for (int i = 1; i <= b.bombPower; i++) {
		int fireType = i == b.bombPower ? 3 : 1;
		if (createFire(fireType, pos.x, pos.y - 48 * i, p2score)) {
			break;
		}
	}
	// Left:
	for (int i = 1; i <= b.bombPower; i++) {
		int fireType = i == b.bombPower ? 6 : 4;
		if (createFire(fireType, pos.x - 48 * i, pos.y, p2score)) {
			break;
		}
	}
	// Right:
	for (int i = 1; i <= b.bombPower; i++) {
		int fireType = i == b.bombPower ? 5 : 4;
		if (createFire(fireType, pos.x + 48 * i, pos.y, p2score)) {
			break;
		}
	}
}

void Level::checkAndFixCollisions(Entity_ptr eCollisioning) {
	sf::Vector2f centerPosition = eCollisioning->getCenterPosition();
	sf::Vector2i position = getMapCoordinates(centerPosition);
	Entity_ptr currentPos;
	if (!Level::isValidCell(position)) {
		std::cout << "Position invalida\n";
	}
	if ((currentPos = getCellMiniMapObject(position)) != nullptr) {
		currentPos->onCollission(eCollisioning, currentPos, CollisionType::NONE);
	}
	//detectar caso
	sf::FloatRect body = eCollisioning->getGlobalBounds();
	float size = body.width / 2;
	sf::Vector2i position_plus = getMapCoordinates(centerPosition.x + size, centerPosition.y + size);
	sf::Vector2i position_minus = getMapCoordinates(centerPosition.x - size, centerPosition.y - size);
	bool horizontal = false;
	bool vertical = false;
	sf::Vector2i position2(position.x, position.y);
	//ver posibilidad colision horizontal
	if (position_plus.x != position.x) {
		//sobre sale por la dch
		position2.x++;
		horizontal = true;
	}
	else if (position_minus.x != position.x) {
		//sobre sale por la izq
		position2.x--;
		horizontal = true;
	}

	//ver posiblida colision vertical
	if (position_plus.y != position.y) {
		//sobre sale por la dch
		position2.y++;
		vertical = true;
	}
	else if (position_minus.y != position.y) {
		//sobre sale por la izq
		position2.y--;
		vertical = true;
	}

	if (!horizontal && !vertical) {
		//Dentro de la celda. Sin colision (Caso 0)
		//No hacer nada
	}
	else if ((!horizontal && vertical) || (horizontal && !vertical)) {
		/******CASOS*************
			 * 	  | x | p |
			 * 	     OR
			 * 	    | p |
			 * 	    | X |
			************************/
		Entity_ptr e;
		if ((e = getCellMiniMapObject(position2.x, position2.y)) != nullptr && e->isColliderWith(eCollisioning)) {
			sf::Vector2f pos = eCollisioning->getPosition();
			if (horizontal) {
				//					pos.x += moveGetOrDie_x(*eCollisioning, e);
				e->onCollission(eCollisioning, e, CollisionType::HORIZONTAL);
			}
			else if (vertical) {
				e->onCollission(eCollisioning, e, CollisionType::VERTICAL);
			}
		}
	}
	else {
		sf::Vector2f RealPos = eCollisioning->getPosition();
		//Posible interseccion en 3 celdas
		Entity_ptr e_horizontal, e_vertical, e_diagonal;
		int numIntersecciones = 0;
		e_horizontal = getCellMiniMapObject(position2.x, position.y);
		if (e_horizontal && !e_horizontal->isColliderWith(eCollisioning)) {
			e_horizontal = nullptr;
		}
		e_vertical = getCellMiniMapObject(position.x, position2.y);
		if (e_vertical && !e_vertical->isColliderWith(eCollisioning)) {
			e_vertical = nullptr;
		}
		e_diagonal = getCellMiniMapObject(position2.x, position2.y);
		if (e_diagonal && !e_diagonal->isColliderWith(eCollisioning)) {
			e_diagonal = nullptr;
		}

		if (e_horizontal && e_vertical) {
			/******CASOS*************
				 * | x | p |
				 * | ? | x |
				 *
				************************/
			e_horizontal->onCollission(eCollisioning, e_horizontal, CollisionType::HORIZONTAL);
			e_vertical->onCollission(eCollisioning, e_vertical, CollisionType::VERTICAL);
		}
		else if (e_diagonal && e_horizontal) {
			/******CASOS*************
				 * | x | p |
				 * | x |   |
			************************/
			e_horizontal->onCollission(eCollisioning, e_horizontal, CollisionType::HORIZONTAL);
		}
		else if (e_diagonal && e_vertical) {
			/******CASOS*************
				 * |   | p |
				 * | x | x |
			************************/
			e_vertical->onCollission(eCollisioning, e_vertical, CollisionType::VERTICAL);
		}
		else {

			Entity_ptr col;
			if (e_horizontal) {
				/******CASOS*************
				 * | x | p |
				 * |   |   |
			************************/
				col = e_horizontal;
			}
			if (e_vertical) {
				/******CASOS*************
				 * |   | p |
				 * |   | x |
				************************/
				col = e_vertical;
			}
			if (e_diagonal) {
				/******CASOS*************
				 * |   | p |
				 * | x |   |
			************************/
				col = e_diagonal;
			}
			if (col) {
				col->onCollission(eCollisioning, col, CollisionType::CORNER);
				RealPos = eCollisioning->getPosition();
			}
		}
	}
}

////////////////////////////////////////////////
/////////	MAP OPERATORS	////////////////////
////////////////////////////////////////////////

sf::Vector2f Level::getMapCellCorner(sf::Vector2f pos) {
	return sf::Vector2f((int)pos.x / SIZE_PILLAR * SIZE_PILLAR, (int)pos.y / SIZE_PILLAR * SIZE_PILLAR);
}

Entity_ptr& Level::getCellObject(int x, int y) {
	if (EntityMap::entityMap[y][x].get() != nullptr && EntityMap::entityMap[y][x].get()->getExpiredEntity()) {
		EntityMap::entityMap[y][x].reset();
	}
	return EntityMap::entityMap[y][x];
}

Entity_ptr& Level::getCellMiniMapObject(int x, int y) {
	if (miniMap[y][x].get() != nullptr && miniMap[y][x].get()->getExpiredEntity()) {
		miniMap[y][x].reset();
	}
	return miniMap[y][x];
}

Entity_ptr& Level::getCellMiniMapObject(sf::Vector2i pos) {
	return getCellMiniMapObject(pos.x, pos.y);
}
Entity_ptr& Level::getCellObject(sf::Vector2i pos) {
	return getCellObject(pos.x, pos.y);
}

void Level::addEntityToMap(Entity_ptr e, int x, int y) {
	getCellObject(x, y) = e;
}

void Level::addEntityToMap(Entity_ptr e, sf::Vector2i pos) {
	addEntityToMap(e, pos.x, pos.y);
}
void Level::addEntityToMiniMap(Entity_ptr e, int x, int y) {
	getCellMiniMapObject(x, y) = e;
}

void Level::addEntityToMiniMap(Entity_ptr e, sf::Vector2i pos) {
	addEntityToMiniMap(e, pos.x, pos.y);
}
void Level::addNewItem(Entity_ptr e) {
	addEntity(e);
	addEntityToMiniMap(e, getMapCoordinates(e->getCenterPosition()));
}

void Level::addEntity(Entity_ptr e) {
	entities.push_back(e);
}

void Level::addPillar(int x, int y) {
	Entity_ptr e = std::make_shared<Pillar>(Pillar(x, y, Level::stage - 1));
	addEntityToMap(e, x, y);
	addEntityToMiniMap(e, x, y);
}

void Level::addWall(int x, int y) {
	Entity_ptr e = std::make_shared<BrickWall>(BrickWall(x, y, Level::stage - 1));
	addEntityToMap(e, x, y);
	addEntityToMiniMap(e, x, y);
	Level::numWalls = numWalls + 1;


}

bool Level::addBomb(Player_ptr p) {
	sf::Vector2f currentPos = Level::getMapCellCorner(p->getCenterPosition());
	if (Level::getCellMiniMapObject(getMapCoordinates(currentPos)) == nullptr) {
		Bomb_ptr b = std::make_shared<Bomb>(Bomb(p));
		b->setPosition(currentPos);
		Level::addNewItem(b);
		b->me = b;
		p->AssignBomb(b);
		return true;
	}
	return false;
}

bool Level::areBombNear(Player_ptr p, sf::Vector2i& bombPosMap, bool checkOnFeet) {

	sf::Vector2f PlayerPos = p->getCenterPosition();
	bombPosMap = getMapCoordinates(PlayerPos);
	//Mirar si hay una bomba a sus pies
	if (checkOnFeet && getCellMiniMapObject(bombPosMap) != nullptr && std::dynamic_pointer_cast<Bomb>(getCellMiniMapObject(bombPosMap)) != nullptr) {
		return true;
	}

	sf::Vector2f takingZone;
	switch (p->lastMovement) {
	case LookingAt::down:
		takingZone.y = SIZE_PILLAR_2;
		break;
	case LookingAt::up:
		takingZone.y = -SIZE_PILLAR_2;
		break;
	case LookingAt::left:
		takingZone.x = -SIZE_PILLAR_2;
		break;
	case LookingAt::right:
		takingZone.x = SIZE_PILLAR_2;
		break;

	default:
		break;
	}

	bombPosMap = getMapCoordinates(PlayerPos + takingZone);

	if (getCellMiniMapObject(bombPosMap) != nullptr && std::dynamic_pointer_cast<Bomb>(getCellMiniMapObject(bombPosMap)) != nullptr) {
		return true;
	}
	return false;
}

bool Level::canTakeBomb(Player_ptr p) {
	sf::Vector2i tankinCell;
	if (areBombNear(p, tankinCell)) {
		Entity_ptr bomb = getCellMiniMapObject(tankinCell);
		p->takeBomb(bomb);
		getCellMiniMapObject(tankinCell).reset();
		return true;
	}
	return false;
}

void Level::ThrowBomb(Player_ptr p, Bomb_ptr b) {
	sf::Vector2i dirThrow;
	switch (p->lastMovement) {
	case LookingAt::down:
		dirThrow.y = SHOOTING_DISTANCE;
		break;
	case LookingAt::up:
		dirThrow.y = -SHOOTING_DISTANCE;
		break;
	case LookingAt::left:
		dirThrow.x = -SHOOTING_DISTANCE;
		break;
	case LookingAt::right:
		dirThrow.x = SHOOTING_DISTANCE;
		break;
	}

	sf::Vector2i cordsP = getMapCoordinates(p->getCenterPosition());
	sf::Vector2i fallPosition = cordsP + dirThrow;
	//Verificar si no sale por los lados
	if (fallPosition.x < 1) { //No puede caer en la columna del borde
		fallPosition.x = 1;
	}
	else if (fallPosition.x > miniMap[0].size() - 1) {
		fallPosition.x = miniMap[0].size() - 2;
	}

	//verificar que no sale en vertical
	if (fallPosition.y < 1) { //No puede caer en la columna del borde
		fallPosition.y = 1;
	}
	else if (fallPosition.y > miniMap.size() - 1) {
		fallPosition.y = miniMap.size() - 2;
	}

	if (fallPosition.x == cordsP.x && fallPosition.y == cordsP.y) {
		addEntityToMiniMap(b, fallPosition); //No puede desplazarse -> se cologa en el suelo
	}
	else {
		onFlightBombs.push_back(b);
		b->setObjetive(MapCoordinates2GlobalCoorCorner(fallPosition));
		b->setPosition(Level::getMapCellCorner(p->getCenterPosition()));
		b->setOnFlight(normalize(dirThrow));

	}
	b->player2Score = p;
}


void Level::reiniciar(int dimX, int dimY) {
	Level::exitHasApeared = false;
	Level::canFinishLevel = false;
	Level::levelFinished = false;
	teleporter = nullptr;
	miniMap.clear();
	miniMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
	for (int x = 0; x < dimX + 2; x++) {
		addPillar(x, 0);
		addPillar(x, dimY + 1);
	}

	for (int y = 0; y < dimY + 2; y++) {
		addPillar(0, y);
		addPillar(dimX + 1, y);
	}

	//create Pillars
	for (int x = 1; x < dimX + 1; x++) {
		for (int y = 1; y < dimY + 1; y++) {
			if (x % 2 == 1 || y % 2 == 1) {
				// Create random Bricks:
				if ((y > 3 || x > 3 || (y != 3 && x == 2) || (x == 3 && y <= 3)) && !Random::getIntNumberBetween(0, 3)) {
					bool intersec = false;
					for (Enemy_ptr e : Enemies::getVectorEnemies()) {
						sf::Vector2i p = getMapCoordinates(e->getCenterPosition());
						if (p.x == x && p.y == y) {
							intersec = true;
							break;
						}
					}
					if (!intersec) {
						addWall(x, y);
					}
				}
			}
			else {
				addPillar(x, y);
			}
		}
	}
}

bool Level::canKickBomb(Player_ptr p) {
	sf::Vector2i tankinCell;
	if (areBombNear(p, tankinCell, false)) {
		Bomb_ptr bomb = std::dynamic_pointer_cast<Bomb>(getCellMiniMapObject(tankinCell));
		sf::Vector2f dirThrow;
		switch (p->lastMovement) {
		case LookingAt::down:
			dirThrow.y = 1;
			break;
		case LookingAt::up:
			dirThrow.y = -1;
			break;
		case LookingAt::left:
			dirThrow.x = -1;
			break;
		case LookingAt::right:
			dirThrow.x = 1;
			break;
		}

		bomb->setOnMove(dirThrow);
		return true;
	}
	return false;
}

void Level::enemiesDefeated() {
	Level::canFinishLevel = true;

	if (teleporter != nullptr) {
		teleporter->openTeleporter();
	}
}

void Level::newEnemiesAppear() {
	Level::canFinishLevel = false;

	if (teleporter != nullptr) {
		teleporter->closeTeleporter();
	}
}

void Level::deleteWall(sf::Vector2i pos) {
	cout << pos.x << "  ---  " << pos.y << endl;
	Entity_ptr e = getCellMiniMapObject(pos);
	BrickWall_ptr b;
	if ((b = std::dynamic_pointer_cast<BrickWall>(e)) != nullptr) {
		//b->setExpiredEntity();
		b->deleteWall();
		
		addEntity(e);
		getCellObject(pos) = nullptr;
		e = nullptr;

		//numWalls--;
		cout << "MURO ENCONTRADO" << endl;
	}
}

void Level::checkSpawn(int posX, int posY) {
	sf::Vector2i pos = getMapCoordinates(posX + SIZE_PILLAR, posY + SIZE_PILLAR);
	Level::deleteWall(pos);
	
	pos = getMapCoordinates(posX - SIZE_PILLAR, posY + SIZE_PILLAR);
	Level::deleteWall(pos);

	pos = getMapCoordinates(posX, posY + SIZE_PILLAR + SIZE_PILLAR);
	Level::deleteWall(pos);

	pos = getMapCoordinates(posX, posY - SIZE_PILLAR + SIZE_PILLAR);
	Level::deleteWall(pos);

	pos = getMapCoordinates(posX, posY + SIZE_PILLAR);
	Level::deleteWall(pos);
}

