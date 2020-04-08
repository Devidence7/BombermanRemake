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


static std::vector<Player_ptr> players;
static std::vector<std::vector<Entity_ptr>> EntityMap;



inline void addPlayer(){
    players.push_back(std::make_shared<PlayerEntity>(PlayerEntity()));
}

inline void insertPlayers(int numPlayers = 1){
    addPlayer();
}
