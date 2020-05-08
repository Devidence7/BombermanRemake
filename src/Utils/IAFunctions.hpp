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


sf::Vector2i generateObjetive2Player(std::shared_ptr<Entity> p, std::shared_ptr<Entity> e,  std::vector<sf::Vector2i> &objetives);

void seekAnyPlayerOrRandom(list<ANode_Ptr> & movements, std::shared_ptr<Entity> e);


//#include "../Include/EntitiesInclude.hpp"
