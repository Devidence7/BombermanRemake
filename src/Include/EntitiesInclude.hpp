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
#include "../Entities/PlayerIA.hpp"
typedef std::shared_ptr<PlayerIAEntity> PlayerIA_ptr;
#include "../Entities/PowerUp.h"
typedef std::shared_ptr<PlayerEntity> PowerUp_ptr;

typedef std::shared_ptr<BrickWall> BrickWall_ptr;
#include "../Entities/Teleporter.h"
typedef std::shared_ptr<Teleporter> Teleporter_ptr;

class PLayers
{
    static std::vector<Player_ptr> players;
    PLayers(){};

public:
    static void addPlayer(PlayerEntity::PlayerControls &playerControls, int team = 0);
    static void addIAPlayer(PlayerEntity::PlayerControls &playerControls,int team = 0, int IA=0);
    static std::vector<Player_ptr> &getVectorPlayer();
    static bool cehckSomeCollision(Entity_ptr e);
};

class Enemies{
    static std::vector<Enemy_ptr> enemies;
    static std::vector<Enemy_ptr> enemiesExtra;
  //   static std::vector<Enemy_ptr> enemiesExtraTel;
   

public:
    //static void insertarEnemigosExtraTel(int dimX, int dimY);
    static void insertarEnemigosExtra(int dimX, int dimY);
    static void insertarEnemigos(int dimX, int dimY,int numEnemies,int stage);
    static std::vector<Enemy_ptr> &getVectorEnemies();
    static std::vector<Enemy_ptr> &getVectorEnemiesExtra();
  //  static std::vector<Enemy_ptr> &getVectorEnemiesExtraTel();
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
    static sf::Vector2i size(){
        return sf::Vector2i(entityMap[0].size(), entityMap.size());
    }
};

