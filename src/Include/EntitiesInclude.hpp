#pragma once

#include <memory>
#include <vector>

#include "../Entities/Entity.h"
typedef std::shared_ptr<Entity> Entity_ptr;
#include "../Entities/Bomb.h"
typedef std::shared_ptr<Bomb> Bomb_ptr;
#include "../Entities/BrickWall.h"
typedef std::shared_ptr<Fire> Fire_ptr;
#include "../Entities/Enemy.h"
typedef std::shared_ptr<EnemyEntity> Enemy_ptr;
#include "../Entities/Pillar.hpp"
typedef std::shared_ptr<Pillar> Pillar_ptr;
#include "../Entities/Player.h"
typedef std::shared_ptr<PlayerEntity> Player_ptr;
#include "../Entities/PowerUp.h"

typedef std::shared_ptr<BrickWall> BrickWall_ptr;

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

