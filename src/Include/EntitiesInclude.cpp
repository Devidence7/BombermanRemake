#include "EntitiesInclude.hpp"

std::vector<Player_ptr> PLayers::players;
std::vector<std::vector<Entity_ptr>> EntityMap::entityMap;


bool checkSomeCollisionOnVector(Entity_ptr e, std::vector<Entity_ptr>& v) {
	bool collision = false;
	sf::FloatRect body = e->getGlobalBounds();
	for (Entity_ptr ev : v) {
		if (collision) { break; }
		collision = body.intersects(ev->getGlobalBounds());
	}
	return collision;
}

void PLayers::addPlayer(PlayerEntity::PlayerControls& playerControls, int posX, int posY, int team) {
	Player_ptr p = std::make_shared<PlayerEntity>(PlayerEntity(playerControls, team, posX, posY));
	p->me = p;
	players.push_back(p);
}

void PLayers::addIAPlayer(PlayerEntity::PlayerControls& playerControls, int posX, int posY, bool debug, int team, int IA) {
	PlayerIA_ptr p = std::make_shared<PlayerIAEntity>(PlayerIAEntity(playerControls, posX, posY, team, IA));
	p->me = p;
	if (team == 0 || team == 1) {
		cout << "Pos x: " << posX << " Pos y: " << posY << endl;
		p->createStateGenerator("../IAFIles/IA1.txt");
	}
	else if (team == 2) {
		p->createStateGenerator("../IAFIles/IA2.txt");
	}
	else {
		p->createStateGenerator("../IAFIles/IA3.txt");
	}
	p->startStates();
	players.push_back(p);
}

std::vector<Player_ptr>& PLayers::getVectorPlayer() {
	return players;
}


bool PLayers::cehckSomeCollision(Entity_ptr e) {
	bool collision = false;
	sf::FloatRect body = e->getGlobalBounds();
	for (Player_ptr ev : players) {
		if (collision) { break; }
		collision = body.intersects(ev->getGlobalBounds());
	}
	return collision;
}


bool Enemies::cehckSomeCollision(Entity_ptr e) {
	bool collision = false;
	sf::FloatRect body = e->getGlobalBounds();
	for (Enemy_ptr ev : enemies) {
		if (collision) { break; }
		collision = body.intersects(ev->getGlobalBounds());
	}
	return collision;
}

std::vector<Enemy_ptr> Enemies::enemies;
std::vector<Enemy_ptr> Enemies::enemiesExtra;
//std::vector<Enemy_ptr> Enemies::enemiesExtra;

/*void Enemies::insertarEnemigosExtraTel(int posX, int posY){
	std::vector<Enemy_ptr> a(7);
	int x, y;
	for(int i=0;i<3;i++){
		a[i] = std::make_shared<EnemyEntity>(Coin());
		enemiesExtra.push_back(a[i]);
	}

	for (Enemy_ptr e : enemiesExtra) {

		//e->setPosition(sf::Vector2f((x * 2 + 1) * SIZE_PILLAR - 3, (y * 2 + 1) * SIZE_PILLAR - 3));
		e->setPosition(MapCoordinates2GlobalCoorCorner(posX,posY));
		e->me=e;
		e->startMovement();
	}

	}*/


void Enemies::insertarEnemigosExtraTeleport(sf::Vector2f pos, int numEnemigos = 3) {
	for (int i = 0; i < numEnemigos; i++) {
		auto a = std::make_shared<EnemyEntity>(Coin(3));
		a->setPosition(pos);
		a->me = a;
		a->setInvencible1sec();
		a->startMovement();
		enemies.push_back(a);
	}
}

void Enemies::insertarEnemigosExtra(int dimX, int dimY, int numEnemigos = 5) {
	int x;
	int y;
	for (int i = 0; i < numEnemigos; i++) {
		auto a = std::make_shared<EnemyEntity>(Coin(3));

		do {
			x = Random::getIntNumberBetween(0, dimX / 2);

		} while (x < 3);
		do {
			y = Random::getIntNumberBetween(0, dimY / 2);
		} while (y < 3);
		//e->setPosition(sf::Vector2f((x * 2 + 1) * SIZE_PILLAR - 3, (y * 2 + 1) * SIZE_PILLAR - 3));
		a->setPosition(MapCoordinates2GlobalCoorCorner(x * 2 + 1, y * 2 + 1));
		a->me = a;
		a->startMovement();
		enemies.push_back(a);
	}

}



void Enemies::insertarEnemigos(int dimX, int dimY, int numEnemies, int stage, float dif) {
	std::vector<Enemy_ptr> a(numEnemies);
	int enemyType;

	double enemySpawnValue = 20;
	enemySpawnValue *= (double)stage * dif;

	auto spawningGrades = vector<double>();

	for (int i = 0; i < numEnemies; i++) {
		spawningGrades.push_back(enemySpawnValue - enemySpawnValue / numEnemies * i);
	}

	for (int i = 0; i < numEnemies; i++) {
		cout << "PROBABIITY " << spawningGrades[i] << endl;

		if (spawningGrades[i] < 20) {
			a[i] = std::make_shared<EnemyEntity>(Balloon(dif));
		}
		else if (spawningGrades[i] < 35) {
			a[i] = std::make_shared<EnemyEntity>(Ice(dif));
		}
		else if (spawningGrades[i] < 45) {
			a[i] = std::make_shared<EnemyEntity>(Barrel(dif));
		}
		else if (spawningGrades[i] < 65) {
			a[i] = std::make_shared<EnemyEntity>(Blob(dif));
		}
		else if (spawningGrades[i] < 90) {
			a[i] = std::make_shared<EnemyEntity>(Ghost(dif));
		}
		else if (spawningGrades[i] < 140) {
			a[i] = std::make_shared<EnemyEntity>(Hypo(dif));
		}
		else {
			a[i] = std::make_shared<EnemyEntity>(Coin(dif));
		}
		enemies.push_back(a[i]);
	}

	auto it = a.begin();
	while (it != a.end()) {
		it->reset();
		it = a.erase(it);
	}
	a.clear();


	for (Enemy_ptr e : enemies) {
		int x, y;
		do {
			x = Random::getIntNumberBetween(0, dimX / 2);

		} while (x < 2);

		y = Random::getIntNumberBetween(0, dimY / 2);
		//e->setPosition(sf::Vector2f((x * 2 + 1) * SIZE_PILLAR - 3, (y * 2 + 1) * SIZE_PILLAR - 3));
		e->setPosition(MapCoordinates2GlobalCoorCorner(x * 2 + 1, y * 2 + 1));
		e->me = e;
		e->startMovement();
	}
}
std::vector<Enemy_ptr>& Enemies::getVectorEnemies() {
	return enemies;
}

std::vector<Enemy_ptr>& Enemies::getVectorEnemiesExtra() {
	return enemiesExtra;
}

void EntityMap::addEntity(Entity_ptr e, sf::Vector2i pos) {
	EntityMap::entityMap[pos.x][pos.y] = e;
}

Entity_ptr& EntityMap::getEntity(sf::Vector2i pos) {
	return EntityMap::entityMap[pos.x][pos.y];
}

Entity_ptr& EntityMap::getCellEntMapObject(int x, int y) {
	if (entityMap[y][x].get() != nullptr && entityMap[y][x].get()->getExpiredEntity()) {
		entityMap[y][x].reset();
	}
	return entityMap[y][x];
}
Entity_ptr& EntityMap::getCellEntMapObject(sf::Vector2i pos) {
	return getCellEntMapObject(pos.x, pos.y);
}

bool EntityMap::isValidCell(sf::Vector2i v) {
	return v.x > -1 && v.y > -1 && entityMap.size() > v.y && entityMap[0].size() > v.x;
}