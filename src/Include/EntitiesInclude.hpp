#pragma once

#include <memory>
#include <vector>

#include "../Entities/Entity.h"
#include "../Entities/Bomb.h"
#include "../Entities/BrickWall.h"
#include "../Entities/Enemy.h"
#include "../Entities/Pillar.hpp"
#include "../Entities/Player.h"
#include "../Entities/PowerUp.h"

typedef std::shared_ptr<Entity> Entity_ptr;
typedef std::shared_ptr<Fire> Fire_ptr;
typedef std::shared_ptr<Pillar> Pillar_ptr;
typedef std::shared_ptr<BrickWall> BrickWall_ptr;
typedef std::shared_ptr<EnemyEntity> Enemy_ptr;
typedef std::shared_ptr<PlayerEntity> Player_ptr;

class PLayers
{
    static std::vector<Player_ptr> players;
    PLayers(){};

public:
    static void addPlayer();
    static void insertPlayers(int numPlayers = 1);
    static std::vector<Player_ptr> &getVectorPlayer();
};

class EntityMap
{
    EntityMap(){};

public:
    static std::vector<std::vector<Entity_ptr>> entityMap;
    static void addEntity(Entity_ptr e, sf::Vector2i pos);
    static Entity_ptr getEntity(sf::Vector2i pos);
    static Entity_ptr &getCellEntMapObject(int x, int y);
    static Entity_ptr &getCellEntMapObject(sf::Vector2i pos);
    static bool isValidCell(sf::Vector2i v);
};

