#include "EntitiesInclude.hpp"

std::vector<Player_ptr> PLayers::players;
std::vector<std::vector<Entity_ptr>> EntityMap::entityMap;

void PLayers::addPlayer()
{
    players.push_back(std::make_shared<PlayerEntity>(PlayerEntity()));
}
void PLayers::insertPlayers(int numPlayers)
{   //cout<<numPlayers<<endl;
    for (int i=0;i<numPlayers;i++){
        addPlayer();
    }
}
std::vector<Player_ptr> &PLayers::getVectorPlayer()
{
    return players;
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

bool EntityMap::isValidCell(sf::Vector2i v){
    return v.x > -1 && v.y > -1 && entityMap.size() > v.y && entityMap[0].size() > v.x;
}