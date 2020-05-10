#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>

#include "A_Star.hpp"
#include "../Entities/Entity.h"
#include "../Include/global.hpp"

/**
 * Return true <-> P is on range
 */ 
inline bool isOnVision(std::shared_ptr<Entity> p, sf::Vector2i PointReference, int RangeVision){
    sf::Vector2i pos = getMapCoordinates(p->getCenterPosition());
    return RangeVision < 0 || (abs(pos.x - PointReference.x) < RangeVision && abs(pos.y - PointReference.y) < RangeVision);
}

inline bool isOnRange(sf::Vector2i pos,sf::Vector2i objetive, int rangeBomb){
    return abs(pos.x - objetive.x) < rangeBomb && abs(pos.x - objetive.x) < rangeBomb;
}


sf::Vector2i generateObjetive2Player(std::shared_ptr<Entity> p, std::shared_ptr<Entity> e,  std::vector<sf::Vector2i> &objetives);

void seekAnyPlayerOrRandom(list<ANode_Ptr> & movements, std::shared_ptr<Entity> e,  TypeSeekIA typeSeek);


//#include "../Include/EntitiesInclude.hpp"

void selectEnemyPlayers(std::shared_ptr<Entity> IA, std::vector<sf::Vector2i> &objetives, int rangeVision);
void tryKillAEnemy(std::shared_ptr<Entity> IA, std::list<ANode_Ptr> &movements, int rangeVision, int costDestroy);
bool somePlayerEnemyOnRange(sf::Vector2i pos, int rangeBomb, int team);