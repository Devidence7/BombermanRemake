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
typedef std::shared_ptr<PlayerEntity> PowerUp_ptr;

typedef std::shared_ptr<BrickWall> BrickWall_ptr;

class PLayers
{
    static std::vector<Player_ptr> players;
    PLayers(){};

public:
    static void addPlayer(PlayerEntity::PlayerControls &playerControls);
    static std::vector<Player_ptr> &getVectorPlayer();
    static bool cehckSomeCollision(Entity_ptr e);
};

class Enemies{
    static std::vector<Enemy_ptr> enemies;

public:
    static void insertarEnemigos(int dimX, int dimY);
    static std::vector<Enemy_ptr> &getVectorEnemies();
    static bool cehckSomeCollision(Entity_ptr e);
};

class EntityMap
{
    EntityMap(){};

public:
    static std::vector<std::vector<Entity_ptr>> entityMap;
    static void addEntity(Entity_ptr e, sf::Vector2i pos);
    static Entity_ptr & getEntity(sf::Vector2i pos);
    static Entity_ptr & getCellEntMapObject(int x, int y);
    static Entity_ptr & getCellEntMapObject(sf::Vector2i pos);
    static bool isValidCell(sf::Vector2i v);
};

