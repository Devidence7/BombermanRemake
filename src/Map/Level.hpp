#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <SFML/Graphics.hpp>
#include <memory>

#include "../Entities/Bomb.h"
#include "../Entities/BrickWall.h"
#include "../Entities/Pillar.hpp"
#include "../Entities/PowerUp.h"
#include "../Textures/WallTexture.h"
#include "../Include/global.hpp"
#include "../Logic/Random.h"

typedef std::shared_ptr<Entity> Entity_ptr;
typedef std::shared_ptr<Bomb> Bomb_ptr;
typedef std::shared_ptr<Fire> Fire_ptr;
typedef std::shared_ptr<Pillar> Pillar_ptr;
typedef std::shared_ptr<BrickWall> BrickWall_ptr;
typedef std::shared_ptr<EnemyEntity> Enemy_ptr;

/**
 *
 * Esta clase se encarga de gestionar la l�gica del juego
 *
 */
class Level
{
	// All entities vector:
	std::vector<Entity_ptr> entities;
	std::vector<Enemy_ptr> enemies;

	// Level map:
	int dimY = 15;
	int dimX = 25;
	std::vector<std::vector<Entity_ptr>> map;
	std::vector<std::vector<Entity_ptr>> miniMap;
	sf::RectangleShape floor;

public:
	Level()
	{
		// Reserve space for faster insert, delete of the entities
		entities.reserve(10000);

		// Create map matrix:
		map = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
		miniMap = std::vector<std::vector<Entity_ptr>>(dimY + 2, std::vector<Entity_ptr>(dimX + 2, nullptr));
		// Background:
		floor.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));
		floor.setFillColor(sf::Color(0, 100, 0));

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

		insertarEnemigos();
		//create Pillars
		for (int x = 1; x < dimX + 1; x++)
		{
			for (int y = 1; y < dimY + 1; y++)
			{
				if (x % 2 == 1 || y % 2 == 1)
				{
					// Create random Bricks:
					if (!Random::getIntNumberBetween(0, 3))
					{
						bool intersec = false;
						for(Enemy_ptr e : enemies){
							sf::Vector2i p = getMapCoordinates(e->getCenterPosition());
							if(p.x == x && p.y == y){
								intersec = true;
								break;
							}
						}
						if(!intersec){
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

	void insertarEnemigos()
	{
		Enemy_ptr e1 = std::make_shared<EnemyEntity>(Balloon());
		Enemy_ptr e2 = std::make_shared<EnemyEntity>(Ice());
		Enemy_ptr e3 = std::make_shared<EnemyEntity>(Barrel());
		Enemy_ptr e4 = std::make_shared<EnemyEntity>(Coin());
		Enemy_ptr e5 = std::make_shared<EnemyEntity>(Blob());
		Enemy_ptr e6 = std::make_shared<EnemyEntity>(Ghost());
		Enemy_ptr e7 = std::make_shared<EnemyEntity>(Hypo());
		enemies.push_back(e1);
		enemies.push_back(e2);
		enemies.push_back(e3);
		enemies.push_back(e4);
		enemies.push_back(e5);
		enemies.push_back(e6);
		enemies.push_back(e7);

		for (Enemy_ptr e : enemies)
		{
			int x = Random::getIntNumberBetween(0, dimX/2);
			int y = Random::getIntNumberBetween(0, dimY/2);
			e->setPosition(sf::Vector2f((x*2 +1) * SIZE_PILLAR, (y*2+1) * SIZE_PILLAR));
		}
	}

	void addPillar(int x, int y)
	{
		Entity_ptr e = std::make_shared<Pillar>(Pillar(x, y));
		addEntityToMap(e, x, y);
		addEntityToMiniMap(e, x, y);
	}

	void addWall(int x, int y)
	{
		Entity_ptr e = std::make_shared<BrickWall>(BrickWall(x, y));
		addEntityToMap(e, x, y);
		addEntityToMiniMap(e, x, y);
	}

	void drawEnemies(sf::RenderWindow& w){
		for(Enemy_ptr &e : this->enemies){
			w.draw(*e);
			if (HITBOX_DEBUG_MODE){

				drawEntityHitbox(w, *e);
			}
		}
	}

	bool colissionWithEnemies(Entity &eCol){
		bool intersec = false;
		for(Enemy_ptr &e : this->enemies){
			intersec = intersec || e->collision(eCol);
		}
		return intersec;
	}

	void updateEnemies()
	{
		auto it = enemies.begin();
		int counter = 0;
		while (it != enemies.end())
		{
			// Update the enemies.
			(*it)->update();
			checkAndFixCollisions2(*(*it));
			if ((*it)->getExpiredEntity())
			{
				it->reset();
				it = enemies.erase(it);
			}
			else
			{
				++it;
				counter++;
			}
		}
	}

	void update()
	{
		auto it = entities.begin();
		int counter = 0;
		// This is made this way because we need to erase element from a vector while we are iterating
		updateEnemies();
		while (it != entities.end())
		{

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

					if (!Random::getIntNumberBetween(0, 3))
					{
						int randomObject = Random::getIntNumberBetween(0, 3);
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
						default:
							powerUp = std::make_shared<MoreSpeedPowerUp>(MoreSpeedPowerUp((*it)->getPosition()));
							break;
						}

						addEntityToMiniMap(powerUp, getMapCoordinates((*it)->getPosition()));
						//addNewItem(powerUp);
					}
					else
					{
						// Do this always?
						this->getCellMiniMapObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
						this->getCellObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
					}
				}
				else
				{
					// Do this always?
					this->getCellMiniMapObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
					this->getCellObject(this->getMapCoordinates((*it)->getPosition())) = nullptr;
				}

				// Remove the entity from the list of entities if it expired.

				it->reset();
				it = entities.erase(it);
			}
			else
			{
				++it;
				counter++;
			}
		}
	}

	/*
	 * This is a DEBUG method, draws in the RenderWindow the hitbox of the Entity
	*/
	void drawEntityHitbox(sf::RenderWindow &w, Entity &e)
	{
		sf::FloatRect dim = e.getGlobalBounds();
		sf::VertexArray lines(sf::Lines, 2);

		// Left
		lines[0].position = sf::Vector2f(dim.left, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		// top
		lines[0].position = sf::Vector2f(dim.left, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		lines[0].position = sf::Vector2f(dim.left, dim.top + dim.height);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);

		lines[0].position = sf::Vector2f(dim.left + dim.width, dim.top);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
		lines[1].color = sf::Color::Red;
		w.draw(lines);
	}

	void draw(sf::RenderWindow &w)
	{
		// Draw the floor:
		w.draw(floor);

		// Draw the entities
		for (std::vector<Entity_ptr> &v : miniMap)
		{
			for (Entity_ptr &e : v)
			{
				if (e != nullptr)
				{
					w.draw(*e);
					if (HITBOX_DEBUG_MODE)
					{
						drawEntityHitbox(w, *e);
					}
				}
			}
		}
		drawEnemies(w);
	}

	void addEntity(Entity_ptr e)
	{
		entities.push_back(e);
	}

	bool createFire(int type, int posX, int posY)
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

	void createFires(Bomb &b)
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

	bool circIntersect(Entity &e1, Entity &e2)
	{
		sf::FloatRect shape1 = e1.getGlobalBounds();
		sf::FloatRect shape2 = e2.getGlobalBounds();

		float dx = (shape1.left + shape1.width / 2) - (shape2.left + (shape2.width / 2));
		float dy = (shape1.top + (shape1.width / 2)) - (shape2.top + (shape2.width / 2));
		float distance = std::sqrt((dx * dx) + (dy * dy));

		return (distance <= (shape1.width / 2) + (shape2.width / 2));
	}

	inline bool dieEntity(Entity &eCollisioning, Entity_ptr &eCollisioner)
	{
		if (std::dynamic_pointer_cast<Fire>(eCollisioner) != nullptr)
		{
			eCollisioning.setExpiredEntity();
			return true;
		}
		else if (dynamic_cast<PlayerEntity *>(&eCollisioning) != nullptr && std::dynamic_pointer_cast<EnemyEntity>(eCollisioner) != nullptr)
		{
			eCollisioning.setExpiredEntity();
			return true;
		}
		return false;
	}

	inline bool getEntity(Entity &eCollisioning, Entity_ptr &eCollisioner)
	{
		PlayerEntity *p;
		std::shared_ptr<PowerUp> pu;
		if ((p = dynamic_cast<PlayerEntity *>(&eCollisioning)) && (pu = std::dynamic_pointer_cast<PowerUp>(eCollisioner)))
		{
			pu->setPlayerStatus(*p);
			pu->setExpiredEntity();
			eCollisioner = nullptr;
			return true;
		}
	}
	//Devuelve true si y solo si eCollisioner hace expirar a eCollisioning o si eCollisioner es un consumible
	// Habra generado una iteraccion de no colision (si existe) entre eCollisioner y eCollisioning
	inline bool getOrDie(Entity &eCollisioning, Entity_ptr &eCollisioner)
	{

		return dieEntity(eCollisioning, eCollisioner) || getEntity(eCollisioning, eCollisioner);
	}

	//Return true if eCollisioning vector with direction new position on X
	inline float moveGetOrDie_x(Entity &eCollisioning, Entity_ptr &eCollisioner)
	{
		if (getOrDie(eCollisioning, eCollisioner))
		{
			return 0;
		}
		sf::Vector2f position = eCollisioning.getCenterPosition();
		sf::FloatRect body = eCollisioning.getGlobalBounds();
		sf::Vector2f position2 = eCollisioner->getCenterPosition();
		sf::FloatRect body2 = eCollisioner->getGlobalBounds();
		float x = calculate_penetration(body.width / 2, body2.width / 2, abs(position.x - position2.x)) + 1;
		if (position.x < position2.x)
		{
			x = -x;
		} //retroceder
		return x;
	}

	inline float moveGetOrDie_y(Entity &eCollisioning, Entity_ptr &eCollisioner)
	{
		if (getOrDie(eCollisioning, eCollisioner))
		{
			return 0;
		}
		sf::Vector2f position = eCollisioning.getCenterPosition();
		sf::FloatRect body = eCollisioning.getGlobalBounds();
		sf::Vector2f position2 = eCollisioner->getCenterPosition();
		sf::FloatRect body2 = eCollisioner->getGlobalBounds();

		float y = calculate_penetration(body.height / 2, body2.height / 2, abs(position.y - position2.y)) + 1;
		if (position.y < position2.y)
		{
			y = -y;
		} //retroceder
		return y;
	}


	//eCirle tiene una colision con eRect. El centro de no está eCricle no está dentro de eRect
	//Devuelve el la direccion (cuyo modulo es la cantidad) al que se tiene que mover la entidad eCircle que esta colisionando con eRect
	inline sf::Vector2f moveOverCorner(Entity &eCircle, float r_Circle, Entity_ptr eRect)
	{

		sf::Vector2f CenterCircle = eCircle.getCenterPosition();
		//sf::FloatRect body = eCircle.getGlobalBounds();
		sf::Vector2f centerRect = eRect->getCenterPosition();
		sf::FloatRect bodyRect = eRect->getGlobalBounds();

		sf::Vector2f dir_centros = centerRect - CenterCircle;
		float distanciaCentros = moduleVector(dir_centros);
		dir_centros = dir_centros / distanciaCentros;
		float intersec_x;
		float intersec_y;

		//Buscar coordenadas referecia
		sf::Vector2f puntoReferenciaRect(bodyRect.left, bodyRect.top);

		if (CenterCircle.y > centerRect.y)
		{
			puntoReferenciaRect.y += bodyRect.height;
		}
		if (CenterCircle.x > centerRect.x)
		{
			puntoReferenciaRect.x += bodyRect.width;
		}
		intersec_x = intersectPoint_planeVector(CenterCircle, dir_centros, sf::Vector2f(1, 0), puntoReferenciaRect);
		intersec_y = intersectPoint_planeVector(CenterCircle, dir_centros, sf::Vector2f(0, 1), puntoReferenciaRect);

		float referencia = 0;
		float intersec = 0;
		if (CenterCircle.x >= bodyRect.left && CenterCircle.x <= bodyRect.left + bodyRect.width)
		{
			referencia = intersec_y;
		}
		else if (CenterCircle.y >= bodyRect.top && CenterCircle.y <= bodyRect.top + bodyRect.height)
		{
			referencia = intersec_x;
		}
		else
		{ //Esquina
			referencia = intersec_x > intersec_y ? intersec_x : intersec_y;
		}
		intersec = ((distanciaCentros - referencia) + r_Circle) - distanciaCentros;

		return (-dir_centros) * intersec;
	}

	bool intersectsCircleRect(Entity &e1circle, Entity &e2rect, double &x, double &y)
	{
		sf::FloatRect circle = e1circle.getGlobalBounds();
		sf::Vector2f circleC = e1circle.getCenterPosition();
		sf::FloatRect rect = e2rect.getGlobalBounds();
		sf::Vector2f recteC = e2rect.getCenterPosition();

		sf::Vector2f circleDistance;

		//std::cout << "1" << std::endl;

		x = (circleC.x - recteC.x);
		y = (circleC.y - recteC.y);
		circleDistance.x = abs(x);
		circleDistance.y = abs(y);

		//std::cout << "2" << std::endl;

		// circle.width and circle.height should be the same (it is the radius).
		if (circleDistance.x > (rect.width / 2 + circle.width / 2))
		{
			return false;
		}
		if (circleDistance.y > (rect.height / 2 + circle.height / 2))
		{
			return false;
		}

		// Move one pixel modulus
		float moduloCentro = sqrtf(x * x + y * y);
		x /= moduloCentro;
		y /= moduloCentro;

		x /= 1000.0;
		y /= 1000.0;

		if (circleDistance.x <= (rect.width / 2))
		{
			return true;
		}
		if (circleDistance.y <= (rect.height / 2))
		{
			return true;
		}

		double cornerDistance_sq;
		cornerDistance_sq = pow((circleDistance.x - rect.width / 2), 2) +
							pow((circleDistance.y - rect.height / 2), 2);

		return (cornerDistance_sq <= pow(circle.width / 2, 2));
	}

	void checkAndFixCollisions2(Entity &eCollisioning)
	{
		sf::Vector2f centerPosition = eCollisioning.getCenterPosition();
		sf::Vector2i position = this->getMapCoordinates(centerPosition);
		Entity_ptr currentPos;
		if ((currentPos = this->getCellMiniMapObject(position)) != nullptr)
		{
			if (this->getOrDie(eCollisioning, currentPos))
			{
				return;
			}
		}
		
		//detectar caso
		sf::FloatRect body = eCollisioning.getGlobalBounds();
		float size = body.width / 2;
		sf::Vector2i position_plus = this->getMapCoordinates(centerPosition.x + size, centerPosition.y + size);
		sf::Vector2i position_minus = this->getMapCoordinates(centerPosition.x - size, centerPosition.y - size);
		bool horizontal = false;
		bool vertical = false;
		sf::Vector2i position2(position.x, position.y);
		//ver posiblida colision horizontal
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
			//Posiblidad de interseccion con 1 solo
			Entity_ptr e;
			if ((e = this->getCellMiniMapObject(position2.x, position2.y)) != nullptr)
			{
				
				//Caso 1.1 o 1.2 1.3 1.4
				sf::Vector2f pos = eCollisioning.getPosition();
				if (horizontal)
				{
					//eCollisioning.setPosition(moveGetOrDie_x(eCollisioning, e) , eCollisioning.getPosition().y);
					pos.x += moveGetOrDie_x(eCollisioning, e);
					if (pos.x != 0)
					{
						eCollisioning.setCollision();
					}
				}
				if (vertical)
				{
					//eCollisioning.setPosition(eCollisioning.getPosition().x, moveGetOrDie_y(eCollisioning, e));
					pos.y += moveGetOrDie_y(eCollisioning, e);
					if (pos.y != 0)
					{
						eCollisioning.setCollision();
					}
				}

			eCollisioning.setPosition(pos);
			}
		
		}else{


		sf::Vector2f RealPos = eCollisioning.getPosition();
		//Posible interseccion en 3 celdas
		Entity_ptr e_horizontal, e_vertical, e_diagonal;
		int numIntersecciones = 0;
		e_horizontal = this->getCellMiniMapObject(position2.x, position.y);
		numIntersecciones++;
		e_vertical = this->getCellMiniMapObject(position.x, position2.y);
		numIntersecciones++;
		e_diagonal = this->getCellMiniMapObject(position2.x, position2.y);
		numIntersecciones++;

		if (e_horizontal && e_vertical)
		{
			//Casos 4 y 5
			RealPos.x += moveGetOrDie_x(eCollisioning, e_horizontal);
			RealPos.y += moveGetOrDie_y(eCollisioning, e_vertical);
			//eCollisioning.setPosition(moveGetOrDie_x(eCollisioning, e_horizontal) , eCollisioning.getPosition().y);
			//eCollisioning.setPosition(eCollisioning.getPosition().x, moveGetOrDie_y(eCollisioning, e_vertical));
		}
		else if (e_diagonal && e_horizontal)
		{
			//caso 3.1
			RealPos.x += moveGetOrDie_x(eCollisioning, e_horizontal);
		}
		else if (e_diagonal && e_vertical)
		{
			//caso 3.2
			RealPos.y += moveGetOrDie_y(eCollisioning, e_vertical);
		}
		else
		{
			//caso2
			//TODO: mejorar
			Entity_ptr col;
			if (e_horizontal)
			{
				col = e_horizontal;
				//		RealPos.x += moveGetOrDie_x(eCollisioning, e_horizontal);
			}
			if (e_vertical)
			{
				col = e_vertical;
				//		RealPos.y += moveGetOrDie_y(eCollisioning, e_vertical);
			}
			if (e_diagonal)
			{
				//		RealPos.x += moveGetOrDie_x(eCollisioning, e_diagonal);
				//		RealPos.y += moveGetOrDie_y(eCollisioning, e_diagonal);
				col = e_diagonal;
			}
			if (!col)
			{
				return;
			} else{

			if (!getOrDie(eCollisioning, col))
			{
				sf::Vector2f move = moveOverCorner(eCollisioning, size, col);
				RealPos.x += move.x;
				RealPos.y += move.y;
				eCollisioning.setCollision();
			}
			}
			
		}
		eCollisioning.setPosition(RealPos);
		if (dynamic_cast<PlayerEntity *>(&eCollisioning) != nullptr){
			for(Enemy_ptr e : enemies){
				if(eCollisioning.collision(*e)){

				}
			}
		}
		}
	}
	// TODO: divide between enemies collition and player collitions:
	void checkAndFixCollisions(Entity &eCollisioning)
	{
		//for (Entity_ptr _e : entities) {

		//for (std::vector<Entity_ptr> &v : miniMap)
		//{
		//	for (Entity_ptr &_e : v)
		sf::Vector2i position = this->getMapCoordinates(eCollisioning.getCenterPosition());
		for (int i = position.y - 1 > 0 ? position.y - 1 : 0; i < position.y + 2; i++)
		{
			for (int j = position.x - 1 > 0 ? position.x - 1 : 0; j < position.x + 2; j++)
			{
				Entity_ptr _e = getCellMiniMapObject(sf::Vector2i(j, i));
				if (_e == nullptr)
				{
					continue;
				}
				double dumb1, dumb2;
				if (intersectsCircleRect(eCollisioning, *_e, dumb1, dumb2))
				{
					eCollisioning.setCollision();
					PlayerEntity *p;
					std::shared_ptr<PowerUp> pu;
					if (std::dynamic_pointer_cast<Fire>(_e) != nullptr)
					{
						eCollisioning.setExpiredEntity();
					}
					else if (dynamic_cast<PlayerEntity *>(&eCollisioning) != nullptr && std::dynamic_pointer_cast<EnemyEntity>(_e) != nullptr)
					{
						eCollisioning.setExpiredEntity();
					}
					else if ((p = dynamic_cast<PlayerEntity *>(&eCollisioning)) && (pu = std::dynamic_pointer_cast<PowerUp>(_e)))
					{
						pu->setPlayerStatus(*p);
						pu->setExpiredEntity();
						_e = nullptr;
					}
					else
					{
						double x;
						double y;
						while (intersectsCircleRect(eCollisioning, *_e, x, y))
						{
							sf::Vector2f blockCpos = _e->getCenterPosition();
							sf::Vector2i mapPos = getMapCoordinates(blockCpos);

							// This is for not guide Bomberman to a hole if he can't go there
							if (abs(x) > abs(y))
							{
								if (y < 0)
								{
									if (getCellObject(mapPos.x, mapPos.y - 1))
									{
										eCollisioning.move(x, 0);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
								else
								{
									if (getCellObject(mapPos.x, mapPos.y + 1))
									{
										eCollisioning.move(x, 0);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
							}
							else
							{
								if (x < 0)
								{
									if (getCellObject(mapPos.x - 1, mapPos.y))
									{
										eCollisioning.move(0, y);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
								else
								{
									if (getCellObject(mapPos.x + 1, mapPos.y))
									{
										eCollisioning.move(0, y);
									}
									else
									{
										eCollisioning.move(x, y);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	sf::Vector2i getMapCoordinates(int x, int y)
	{
		return sf::Vector2i((int)x / SIZE_PILLAR, (int)y / SIZE_PILLAR);
	}

	sf::Vector2i getMapCoordinates(sf::Vector2f pos)
	{
		return getMapCoordinates(pos.x, pos.y);
	}

	sf::Vector2f getMapCellCorner(sf::Vector2f pos)
	{
		return sf::Vector2f((int)pos.x / SIZE_PILLAR * SIZE_PILLAR, (int)pos.y / SIZE_PILLAR * SIZE_PILLAR);
	}

	Entity_ptr &getCellObject(int x, int y)
	{
		if (map[y][x].get() != nullptr && map[y][x].get()->getExpiredEntity())
		{
			map[y][x].reset();
		}
		return map[y][x];
	}

	Entity_ptr &getCellMiniMapObject(int x, int y)
	{
		if (miniMap[y][x].get() != nullptr && miniMap[y][x].get()->getExpiredEntity())
		{
			miniMap[y][x].reset();
		}
		return miniMap[y][x];
	}

	Entity_ptr &getCellMiniMapObject(sf::Vector2i pos)
	{
		return getCellMiniMapObject(pos.x, pos.y);
	}
	Entity_ptr &getCellObject(sf::Vector2i pos)
	{
		return getCellObject(pos.x, pos.y);
	}

	void addEntityToMap(Entity_ptr e, int x, int y)
	{
		getCellObject(x, y) = e;
	}

	void addEntityToMap(Entity_ptr e, sf::Vector2i pos)
	{
		addEntityToMap(e, pos.x, pos.y);
	}
	void addEntityToMiniMap(Entity_ptr e, int x, int y)
	{
		getCellMiniMapObject(x, y) = e;
	}

	void addEntityToMiniMap(Entity_ptr &e, sf::Vector2i pos)
	{
		addEntityToMiniMap(e, pos.x, pos.y);
	}
	void addNewItem(Entity_ptr &e)
	{
		addEntity(e);
		addEntityToMiniMap(e, getMapCoordinates(e->getCenterPosition()));
	}
};