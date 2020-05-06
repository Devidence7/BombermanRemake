#include "Level.hpp"

std::vector<Entity_ptr> Level::entities;
std::vector<Bomb_ptr> Level::onFlightBombs;
std::vector<std::vector<Entity_ptr>> Level::miniMap;
sf::RectangleShape Level::flooro;

Level::Level(int dimX, int dimY)
{
	// Reserve space for faster insert, delete of the entities
	entities.reserve(10000);

	// Create map matrix:
	EntityMap::entityMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
	miniMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
	// Background:
	flooro.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));
	flooro.setFillColor(sf::Color(0, 100, 0));

	// Create all pillars:
	for (int x = 0; x < dimX + 2; x++)
	{
		addPillar(x, 0);
		addPillar(x, dimY + 1);
	}

	for (int y = 0; y < dimY + 2; y++)
	{
		addPillar(0, y);
		addPillar(dimX + 1, y);
	}

	//create Pillars
	for (int x = 1; x < dimX + 1; x++)
	{
		for (int y = 1; y < dimY + 1; y++)
		{
			if (x % 2 == 1 || y % 2 == 1)
			{
				// Create random Bricks:
				if ((y > 3 || x > 3 || (y != 3 && x == 2) || (x == 3 && y <= 3)) && !Random::getIntNumberBetween(0, 3))
				{
					bool intersec = false;
					for (Enemy_ptr e : Enemies::getVectorEnemies())
					{
						sf::Vector2i p = getMapCoordinates(e->getCenterPosition());
						if (p.x == x && p.y == y)
						{
							intersec = true;
							break;
						}
					}
					if (!intersec)
					{
						addWall(x, y);
					}
				}
			}
			else
			{
				addPillar(x, y);
			}
		}
	}
}

void Level::chechAndFixBombCollision(Bomb_ptr b)
{
	sf::FloatRect body = b->getGlobalBoundsCollisioner();
	sf::Vector2f v = normalize(b->getVelocity());
	sf::Vector2f add2Pos(body.height / 2 * v.x, body.height / 2 * v.y);
	sf::Vector2f globalPos = b->getCenterPosition();
	sf::Vector2i mapPosition = getMapCoordinates(globalPos);

	//Eliminar bomba de mapa
	//Curent position
	if (getCellMiniMapObject(mapPosition) == b)
	{
		getCellMiniMapObject(mapPosition).reset();
	}
	//Verificar 2 coordendas
	sf::Vector2i mapPos2 = getMapCoordinates(globalPos - add2Pos);
	if (getCellMiniMapObject(mapPos2) == b)
	{
		getCellMiniMapObject(mapPos2).reset();
	}
	mapPos2 = getMapCoordinates(globalPos + add2Pos);
	if (getCellMiniMapObject(mapPos2) == b)
	{
		getCellMiniMapObject(mapPos2).reset();
	}

	//Mirar colision
	if (getCellMiniMapObject(mapPos2) == nullptr)
	{

		getCellMiniMapObject(mapPos2) = b;
	}
	else
	{
		b->setCollision();
		b->setPosition(getMapCellCorner(globalPos));
	}
	getCellMiniMapObject(mapPosition) = b;//Volver a poner bomb
}

	void Level::update()
	{
		auto it = entities.begin();
		int counter = 0;
		// This is made this way because we need to erase element from a vector while we are iterating
		//std::cout << "Entreando a update\n";
		while (it != entities.end())
		{
			sf::FloatRect beginRect = (*it)->getGlobalBoundsCollisioner();
			sf::Vector2f begintPos = (*it)->getCenterPosition();
			// Update the entities.
			(*it)->update();
			if ((*it)->getExpiredEntity())
			{
				// If it is a bomb
				std::shared_ptr<Bomb> b;
				if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr)
				{
					// Create fires
					createFires(*b);
					// When adding entities vector iterator can be invalidated:
					it = entities.begin() + counter;
				}
				else if (std::dynamic_pointer_cast<BrickWall>((*it)) != nullptr)
				{
					if (!Random::getIntNumberBetween(0, 1))
					{
						int randomObject = Random::getIntNumberBetween(0, 9);
						Entity_ptr powerUp;
						switch (randomObject)
						{
						case 0:
							powerUp = std::make_shared<MoreFirePowerUp>(MoreFirePowerUp((*it)->getPosition()));
							break;
						case 1:
							powerUp = std::make_shared<LessFirePowerUp>(LessFirePowerUp((*it)->getPosition()));
							break;
						case 2:
							powerUp = std::make_shared<MoreBombsPowerUp>(MoreBombsPowerUp((*it)->getPosition()));
							break;
						case 3:
							powerUp = std::make_shared<LessSpeedPowerUp>(LessSpeedPowerUp((*it)->getPosition()));
							break;
						case 4:
							powerUp = std::make_shared<MoreTimePowerUp>(MoreTimePowerUp((*it)->getPosition()));
							break;
						case 5:
							powerUp = std::make_shared<GrabBombsPowerUp>(GrabBombsPowerUp((*it)->getPosition()));
							break;
						case 6:
							powerUp = std::make_shared<KickBombsPowerUp>(KickBombsPowerUp((*it)->getPosition()));
							break;
						case 7:
							powerUp = std::make_shared<DisseasePowerUp>(DisseasePowerUp((*it)->getPosition()));
							break;
						case 8:
							powerUp = std::make_shared<PassBombsPowerUp>(PassBombsPowerUp((*it)->getPosition()));
							break;
						default:
							powerUp = std::make_shared<ExtraLifePowerUp>(ExtraLifePowerUp((*it)->getPosition()));
							break;
						}

						addEntityToMiniMap(powerUp, getMapCoordinates((*it)->getPosition()));
						//addNewItem(powerUp);
					}
					else
					{
						// Do this always?
						getCellMiniMapObject(getMapCoordinates((*it)->getPosition())).reset();
						getCellObject(getMapCoordinates((*it)->getPosition())).reset();
					}
				}
				else
				{
					// Do this always?
					getCellMiniMapObject(getMapCoordinates((*it)->getPosition())).reset();
					getCellObject(getMapCoordinates((*it)->getPosition())).reset();
				}

				// Remove the entity from the list of entities if it expired.

				it->reset();
				it = entities.erase(it);
			}
			else
			{
				std::shared_ptr<Bomb> b;
				if ((b = std::dynamic_pointer_cast<Bomb>(*it)) != nullptr)
				{
					//std::cout << "actulizando item counter " << counter <<"\n";
					//std::cout << "P " << (*it) <<"\n";

					sf::Vector2f positionBomb = b->getPosition();
					sf::Vector2i positionMap = getMapCoordinates(positionBomb);

					if (b->rePutBomb)
					{
						Entity_ptr e;
						if ((e = getCellMiniMapObject(positionMap)) == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr)
						{
							b->rePutBomb = false;
							addEntityToMiniMap((*it), getMapCoordinates(b->getCenterPosition()));
						}
						else
						{
							b->onFlight = true;
							sf::Vector2f v = normalize(b->getVelocity());
							sf::Vector2i vi = sf::Vector2i(v.x, v.y);
							sf::Vector2i newPos = positionMap + vi;
							//std::cout << "NP " << newPos.x << " " << newPos.y << " SzX " << miniMap[0].size() << endl;
							if (newPos.x < 1 || newPos.y < 1 || (miniMap.size() - 2) < newPos.y || (miniMap[0].size() - 2) < newPos.x)
							{ //Si se sale del mapa, rebotar
								vi = -vi;
								v = -v;
								b->setVelocity(v);
								//std::cout << "Fuera de rango\n";
							}

							b->setObjetive(MapCoordinates2GlobalCoorCorner(positionMap + vi));
						}
					}
					else if (b->onMove)
					{
						chechAndFixBombCollision(b);
					}
				}
				it++;
				counter++;
			}
		}
	}

	/*
	 * This is a DEBUG method, draws in the RenderWindow the hitbox of the Entity
	*/

	void Level::draw(sf::RenderWindow & w)
	{
		// Draw the flooro:
		w.draw(flooro);

		// Draw the entities
		for (std::vector<Entity_ptr> &v : miniMap)
		{
			for (Entity_ptr &e : v)
			{
				if (e != nullptr)
				{
					w.draw(*e);
#ifdef HITBOX_DEBUG_MODE
					e->drawEntityHitbox(w);
#endif
				}
			}
		}

		auto it = onFlightBombs.begin();
		// This is made this way because we need to erase element from a vector while we are iterating
		while (it != onFlightBombs.end())
		{
			if (!(*it)->onFlight)
			{
				// Remove the bomb from the list onFlightBombs if it at floor.
				it->reset();
				it = onFlightBombs.erase(it);
			}
			else
			{
				w.draw(*(*it));
				++it;
			}
		}
	}

	bool Level::createFire(int type, int posX, int posY)
	{
		// Get the object in cell
		Entity_ptr e = getCellMiniMapObject(getMapCoordinates(posX, posY));

		if (e && e->getIsFireDestroyable() && !e->getFireCanGoThroght())
		{
			e->setExpiredEntity();
			addEntity(e);
		}

		if (!e || e->getFireCanGoThroght())
		{
			e = nullptr;
			Entity_ptr f = std::make_shared<Fire>(Fire(type));
			f->setPosition(posX, posY);
			addNewItem(f);
			//addEntity(f);
			//addEntityToMiniMap(f, posX, posY);
			return false;
		}
		return true;
	}

	void Level::createFires(Bomb & b)
	{
		sf::Vector2f pos = b.getPosition();

		// Central:
		createFire(0, pos.x, pos.y);
		// Down:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 2 : 1;
			if (createFire(fireType, pos.x, pos.y + 48 * i))
			{
				break;
			}
		}
		// Up:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 3 : 1;
			if (createFire(fireType, pos.x, pos.y - 48 * i))
			{
				break;
			}
		}
		// Left:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 6 : 4;
			if (createFire(fireType, pos.x - 48 * i, pos.y))
			{
				break;
			}
		}
		// Right:
		for (int i = 1; i <= b.bombPower; i++)
		{
			int fireType = i == b.bombPower ? 5 : 4;
			if (createFire(fireType, pos.x + 48 * i, pos.y))
			{
				break;
			}
		}
	}

	void Level::checkAndFixCollisions(Entity_ptr eCollisioning)
	{
		sf::Vector2f centerPosition = eCollisioning->getCenterPosition();
		sf::Vector2i position = getMapCoordinates(centerPosition);
		Entity_ptr currentPos;
		if ((currentPos = getCellMiniMapObject(position)) != nullptr)
		{
			//getOrDie(*eCollisioning, currentPos);
			currentPos->onCollission(eCollisioning, CollisionType::NONE);
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
		if (position_plus.x != position.x)
		{
			//sobre sale por la dch
			position2.x++;
			horizontal = true;
		}
		else if (position_minus.x != position.x)
		{
			//sobre sale por la izq
			position2.x--;
			horizontal = true;
		}

		//ver posiblida colision vertical
		if (position_plus.y != position.y)
		{
			//sobre sale por la dch
			position2.y++;
			vertical = true;
		}
		else if (position_minus.y != position.y)
		{
			//sobre sale por la izq
			position2.y--;
			vertical = true;
		}

		if (!horizontal && !vertical)
		{
			//Dentro de la celda. Sin colision (Caso 0)
			//No hacer nada
		}
		else if ((!horizontal && vertical) || (horizontal && !vertical))
		{
			/******CASOS*************
			 * 	  | x | p |  
			 * 	     OR    
			 * 	    | p |
			 * 	    | X |
			************************/
			Entity_ptr e;
			if ((e = getCellMiniMapObject(position2.x, position2.y)) != nullptr)
			{
				sf::Vector2f pos = eCollisioning->getPosition();
				if (horizontal)
				{
					//					pos.x += moveGetOrDie_x(*eCollisioning, e);
					e->onCollission(eCollisioning, CollisionType::HORIZONTAL);
				}
				else if (vertical)
				{
					e->onCollission(eCollisioning, CollisionType::VERTICAL);
				}
			}
		}
		else
		{
			sf::Vector2f RealPos = eCollisioning->getPosition();
			//Posible interseccion en 3 celdas
			Entity_ptr e_horizontal, e_vertical, e_diagonal;
			int numIntersecciones = 0;
			e_horizontal = getCellMiniMapObject(position2.x, position.y);
			numIntersecciones++;
			e_vertical = getCellMiniMapObject(position.x, position2.y);
			numIntersecciones++;
			e_diagonal = getCellMiniMapObject(position2.x, position2.y);
			numIntersecciones++;

			if (e_horizontal && e_vertical)
			{
				/******CASOS*************
				 * | x | p |
				 * | ? | x |
				 * 
				************************/
				e_horizontal->onCollission(eCollisioning, CollisionType::HORIZONTAL);
				e_vertical->onCollission(eCollisioning, CollisionType::VERTICAL);
			}
			else if (e_diagonal && e_horizontal)
			{
				/******CASOS*************
				 * | x | p |
				 * | x |   |
			************************/
				e_horizontal->onCollission(eCollisioning, CollisionType::HORIZONTAL);
			}
			else if (e_diagonal && e_vertical)
			{
				/******CASOS*************
				 * |   | p |
				 * | x | x |
			************************/
				e_vertical->onCollission(eCollisioning, CollisionType::VERTICAL);
			}
			else
			{

				Entity_ptr col;
				if (e_horizontal)
				{
					/******CASOS*************
				 * | x | p |
				 * |   |   |
			************************/
					col = e_horizontal;
				}
				if (e_vertical)
				{
					/******CASOS*************
				 * |   | p |
				 * |   | x |
				************************/
					col = e_vertical;
				}
				if (e_diagonal)
				{
					/******CASOS*************
				 * |   | p |
				 * | x |   |
			************************/
					col = e_diagonal;
				}
				if (!col)
				{	//Si no hay colision -> fin
					//return;
				}
				else
				{
					col->onCollission(eCollisioning, CollisionType::CORNER);
					RealPos = eCollisioning->getPosition();
				}
			}
		}
	}

	////////////////////////////////////////////////
	/////////	MAP OPERATORS	////////////////////
	////////////////////////////////////////////////

	sf::Vector2f Level::getMapCellCorner(sf::Vector2f pos)
	{
		return sf::Vector2f((int)pos.x / SIZE_PILLAR * SIZE_PILLAR, (int)pos.y / SIZE_PILLAR * SIZE_PILLAR);
	}

	Entity_ptr &Level::getCellObject(int x, int y)
	{
		if (EntityMap::entityMap[y][x].get() != nullptr && EntityMap::entityMap[y][x].get()->getExpiredEntity())
		{
			EntityMap::entityMap[y][x].reset();
		}
		return EntityMap::entityMap[y][x];
	}

	Entity_ptr &Level::getCellMiniMapObject(int x, int y)
	{

		if (miniMap[y][x].get() != nullptr && miniMap[y][x].get()->getExpiredEntity())
		{
			miniMap[y][x].reset();
		}
		return miniMap[y][x];
	}

	Entity_ptr &Level::getCellMiniMapObject(sf::Vector2i pos)
	{
		return getCellMiniMapObject(pos.x, pos.y);
	}
	Entity_ptr &Level::getCellObject(sf::Vector2i pos)
	{
		return getCellObject(pos.x, pos.y);
	}

	void Level::addEntityToMap(Entity_ptr e, int x, int y)
	{
		getCellObject(x, y) = e;
	}

	void Level::addEntityToMap(Entity_ptr e, sf::Vector2i pos)
	{
		addEntityToMap(e, pos.x, pos.y);
	}
	void Level::addEntityToMiniMap(Entity_ptr e, int x, int y)
	{
		getCellMiniMapObject(x, y) = e;
	}

	void Level::addEntityToMiniMap(Entity_ptr e, sf::Vector2i pos)
	{
		addEntityToMiniMap(e, pos.x, pos.y);
	}
	void Level::addNewItem(Entity_ptr e)
	{
		addEntity(e);
		addEntityToMiniMap(e, getMapCoordinates(e->getCenterPosition()));
	}

	void Level::addEntity(Entity_ptr e)
	{
		entities.push_back(e);
	}

	void Level::addPillar(int x, int y)
	{
		Entity_ptr e = std::make_shared<Pillar>(Pillar(x, y));
		addEntityToMap(e, x, y);
		addEntityToMiniMap(e, x, y);
	}

	void Level::addWall(int x, int y)
	{
		Entity_ptr e = std::make_shared<BrickWall>(BrickWall(x, y));
		addEntityToMap(e, x, y);
		addEntityToMiniMap(e, x, y);
	}

	bool Level::addBomb(Player_ptr p)
	{
		sf::Vector2f currentPos = Level::getMapCellCorner(p->getCenterPosition());
		if (Level::getCellMiniMapObject(getMapCoordinates(currentPos)) == nullptr)
		{
			Entity_ptr b = std::make_shared<Bomb>(Bomb(p));
			b->setPosition(currentPos);
			Level::addNewItem(b);
			return true;
		}
		return false;
	}

	bool Level::areBombNear(Player_ptr p, sf::Vector2i & bombPosMap)
	{

		sf::Vector2f PlayerPos = p->getCenterPosition();
		bombPosMap = getMapCoordinates(PlayerPos);
		//Mirar si hay una bomba a sus pies
		if (getCellMiniMapObject(bombPosMap) != nullptr && std::dynamic_pointer_cast<Bomb>(getCellMiniMapObject(bombPosMap)) != nullptr)
		{
			return true;
		}

		sf::Vector2f takingZone;
		switch (p->lastMovement)
		{
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

		if (getCellMiniMapObject(bombPosMap) != nullptr && std::dynamic_pointer_cast<Bomb>(getCellMiniMapObject(bombPosMap)) != nullptr)
		{
			return true;
		}
		return false;
	}

	bool Level::canTakeBomb(Player_ptr p)
	{
		sf::Vector2i tankinCell;
		if (areBombNear(p, tankinCell))
		{
			Entity_ptr bomb = getCellMiniMapObject(tankinCell);
			p->takeBomb(bomb);
			getCellMiniMapObject(tankinCell).reset();
			return true;
		}
		return false;
	}

	void Level::ThrowBomb(Player_ptr p, Bomb_ptr b)
	{
		sf::Vector2i dirThrow;
		switch (p->lastMovement)
		{
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
		if (fallPosition.x < 1)
		{ //No puede caer en la columna del borde
			fallPosition.x = 1;
		}
		else if (fallPosition.x > miniMap[0].size() - 1)
		{
			fallPosition.x = miniMap[0].size() - 2;
		}

		//verificar que no sale en vertical
		if (fallPosition.y < 1)
		{ //No puede caer en la columna del borde
			fallPosition.y = 1;
		}
		else if (fallPosition.y > miniMap.size() - 1)
		{
			fallPosition.y = miniMap.size() - 2;
		}

		if (fallPosition.x == cordsP.x && fallPosition.y == cordsP.y)
		{
			addEntityToMiniMap(b, fallPosition); //No puede desplazarse -> se cologa en el suelo
		}
		else
		{
			onFlightBombs.push_back(b);
			b->setObjetive(MapCoordinates2GlobalCoorCorner(fallPosition));
			b->setOnFlight(normalize(dirThrow));
		}
	}

	void Level::reiniciar(int dimX, int dimY)
	{
		miniMap.clear();
		miniMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
		for (int x = 0; x < dimX + 2; x++)
		{
			addPillar(x, 0);
			addPillar(x, dimY + 1);
		}

		for (int y = 0; y < dimY + 2; y++)
		{
			addPillar(0, y);
			addPillar(dimX + 1, y);
		}

		//create Pillars
		for (int x = 1; x < dimX + 1; x++)
		{
			for (int y = 1; y < dimY + 1; y++)
			{
				if (x % 2 == 1 || y % 2 == 1)
				{
					// Create random Bricks:
					if ((y > 3 || x > 3 || (y != 3 && x == 2) || (x == 3 && y <= 3)) && !Random::getIntNumberBetween(0, 3))
					{
						bool intersec = false;
						for (Enemy_ptr e : Enemies::getVectorEnemies())
						{
							sf::Vector2i p = getMapCoordinates(e->getCenterPosition());
							if (p.x == x && p.y == y)
							{
								intersec = true;
								break;
							}
						}
						if (!intersec)
						{
							addWall(x, y);
						}
					}
				}
				else
				{
					addPillar(x, y);
				}
			}
		}
	}

	bool Level::canKickBomb(Player_ptr p)
	{
		sf::Vector2i tankinCell;
		if (areBombNear(p, tankinCell))
		{
			Bomb_ptr bomb = std::dynamic_pointer_cast<Bomb>(getCellMiniMapObject(tankinCell));
			sf::Vector2f dirThrow;
			switch (p->lastMovement)
			{
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
	//TODO: Asociar puntuacion a jugador que lanza
