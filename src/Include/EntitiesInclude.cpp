#include "EntitiesInclude.hpp"

std::vector<Player_ptr> PLayers::players;
std::vector<std::vector<Entity_ptr>> EntityMap::entityMap;

void PLayers::addPlayer()
{
    Player_ptr p = std::make_shared<PlayerEntity>(PlayerEntity());
    p->me = p;
    players.push_back(p);
}
void PLayers::insertPlayers(int numPlayers)
{   cout<<numPlayers<<endl;
    for (int i=0;i<numPlayers;i++){
        addPlayer();
    }
}
std::vector<Player_ptr> &PLayers::getVectorPlayer()
{
    return players;
}

std::vector<Enemy_ptr> Enemies::enemies;
void Enemies::insertarEnemigos(int dimX, int dimY)
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
        int x, y;
        do
        {
            x = Random::getIntNumberBetween(0, dimX / 2);

        } while (x < 3);
        do
        {
            y = Random::getIntNumberBetween(0, dimY / 2);
        } while (y < 3);
        e->setPosition(sf::Vector2f((x * 2 + 1) * SIZE_PILLAR - 3, (y * 2 + 1) * SIZE_PILLAR - 3));
        e->startMovement();
    }
}
std::vector<Enemy_ptr> &Enemies::getVectorEnemies()
{
    return enemies;
}

void EntityMap::addEntity(Entity_ptr e, sf::Vector2i pos)
{
    EntityMap::entityMap[pos.x][pos.y] = e;
}
Entity_ptr EntityMap::getEntity(sf::Vector2i pos)
{
    return EntityMap::entityMap[pos.x][pos.y];
}

Entity_ptr &EntityMap::getCellEntMapObject(int x, int y)
{
    if (entityMap[y][x].get() != nullptr && entityMap[y][x].get()->getExpiredEntity())
    {
        entityMap[y][x].reset();
    }
    return entityMap[y][x];
}
Entity_ptr &EntityMap::getCellEntMapObject(sf::Vector2i pos)
{
    return getCellEntMapObject(pos.x, pos.y);
}

bool EntityMap::isValidCell(sf::Vector2i v)
{
    return v.x > -1 && v.y > -1 && entityMap.size() > v.y && entityMap[0].size() > v.x;
}