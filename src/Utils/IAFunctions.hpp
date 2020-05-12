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

inline bool isOnVision(sf::Vector2i pos, sf::Vector2i PointReference, int RangeVision){
    return RangeVision < 0 || (abs(pos.x - PointReference.x) < RangeVision && abs(pos.y - PointReference.y) < RangeVision);
}


inline bool isOnRange(sf::Vector2i pos,sf::Vector2i objetive, int rangeBomb){
    return abs(pos.x - objetive.x) < rangeBomb && abs(pos.y - objetive.y) < rangeBomb;
}


sf::Vector2i generateObjetive2Player(std::shared_ptr<Entity> p, std::shared_ptr<Entity> e,  std::vector<sf::Vector2i> &objetives);

void seekAnyPlayerOrRandom(list<ANode_Ptr> & movements, std::shared_ptr<Entity> e,  TypeSeekIA typeSeek);

inline void createRangeVision(sf::Vector2i &from, sf::Vector2i &to, sf::Vector2i position, int rangeVision);


//#include "../Include/EntitiesInclude.hpp"
#include "../Entities/Player.h"
void generateOmitedZoneByBomb(sf::Vector2i bombPosition, std::list<OmittedArea> &AreasOmited, float timeBomb);
void generateOmitedZones(sf::Vector2i positionP, std::list<OmittedArea> &AreasOmited, int rangeVision);
void selectEnemyPlayers(std::shared_ptr<Entity> IA, std::vector<sf::Vector2i> &objetives, int rangeVision);
void tryKillAEnemy(std::shared_ptr<Entity> IA, std::list<ANode_Ptr> &movements, int rangeVision, int costDestroy);
bool somePlayerEnemyOnVision(sf::Vector2i pos, int rangeVision, int team);
bool somePlayerEnemyOnRange(sf::Vector2i pos, int rangeBomb, int team);
bool canPutABombSafe(sf::Vector2i posBomb, std::shared_ptr<PlayerEntity> e, std::list<ANode_Ptr> &movements);
Interst_ptr generateIntersetPointDestroyer(sf::Vector2i posPossibleBom);
bool pathFinderDestroy2Farm(const sf::Vector2i &positionEnemy, std::list<ANode_Ptr> &path, std::shared_ptr<Entity>  e, int costAddDestroy);

#include "OmittedAreas.hpp"


class PointsDestroyMap{
    static std::vector<std::vector<Interst_ptr>> interestingMap;
    
    public:
    static Interst_ptr &getIntersetZone(sf::Vector2i pos);
    static Interst_ptr &getIntersetZone(int x, int y);
    static Interst_ptr &addInterset(sf::Vector2i pos, Interst_ptr in);    
    static void addInterset(int x, int y, Interst_ptr in);
    static void updateMap();
    static void resetMap();
};