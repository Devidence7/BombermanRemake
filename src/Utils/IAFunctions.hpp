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

bool checkIsDirectAccesible(sf::Vector2i posA, sf::Vector2i posB);

inline bool isOnRangeExplosion(sf::Vector2i pos, sf::Vector2f centerPosition, sf::FloatRect body, int rangeBomb){
    sf::Vector2i centerMap = getMapCoordinates(centerPosition);
    sf::Vector2i AnchoMax = getMapCoordinates(centerPosition + sf::Vector2f(body.width/2 + 2, 0));
    sf::Vector2i AnchoMin = getMapCoordinates(centerPosition - sf::Vector2f(body.width/2 + 2, 0));
    sf::Vector2i AltoMax = getMapCoordinates(centerPosition + sf::Vector2f(0, body.height/2 + 2 ));
    sf::Vector2i AltoMin = getMapCoordinates(centerPosition - sf::Vector2f(0, body.height/2 + 2 ));

    int posCol = -1; //-1 = noCol;
    bool onRange = (abs(pos.x - centerMap.x) <= rangeBomb && abs(pos.y - centerMap.y) == 0 )    || (abs(pos.y - centerMap.y) <= rangeBomb && abs(pos.x - centerMap.x) == 0);
    if(onRange){posCol = 0;} //0 = center
    onRange = onRange || (abs(pos.x - AnchoMax.x) <= rangeBomb && abs(pos.y - AnchoMax.y) == 0) || (abs(pos.y -  AnchoMax.y) <= rangeBomb && abs(pos.x -  AnchoMax.x) == 0);
    if(onRange && posCol <0){posCol = 1;} // 1 = dch
    onRange = onRange || (abs(pos.x - AnchoMin.x) <= rangeBomb && abs(pos.y - AnchoMin.y) == 0) || (abs(pos.y -  AnchoMin.y) <= rangeBomb && abs(pos.x -  AnchoMin.x) == 0);
    if(onRange && posCol <0){posCol = 2;} //2 = izq
    onRange = onRange || (abs(pos.x -  AltoMax.x) <= rangeBomb && abs(pos.y -  AltoMax.y) == 0) || (abs(pos.y -   AltoMax.y) <= rangeBomb && abs(pos.x -   AltoMax.x) == 0);
    if(onRange && posCol <0){posCol = 3;} //3 = down
    onRange = onRange || (abs(pos.x -  AltoMin.x) <= rangeBomb && abs(pos.y -  AltoMin.y) == 0) || (abs(pos.y -   AltoMin.y) <= rangeBomb && abs(pos.x -   AltoMin.x) == 0);
    if(onRange && posCol <0){posCol = 4;} //4 = up
    if(!onRange){
        return false;
    }
    sf::Vector2i pos2Check;
    switch (posCol)
    {
    case 0:
        pos2Check = centerMap;
        break;
    case 1:
        pos2Check = AnchoMax;
        break;
    case 2:
        pos2Check = AnchoMin;
        break;
    case 3:
        pos2Check = AltoMax;
        break;
    case 4:
        pos2Check = AltoMin;
        break;
    
    default:
        break;
    }
    return onRange && checkIsDirectAccesible(pos2Check, pos);
}

// timeToPutNewBomb < 0 -> no quiere poner bomba
bool WillDead(sf::Vector2i posPLayer, float timeToPutNewBomb);
int getFiresOnPlayer(sf::Vector2i posPLayer, float timeToPutNewBomb);

inline bool isOnRange(sf::Vector2i pos,sf::Vector2i objetive, int rangeBomb){
    return abs(pos.x - objetive.x) <= rangeBomb && abs(pos.y - objetive.y) <= rangeBomb;
}

inline bool isOnRangeBomb(sf::Vector2i pos,sf::Vector2i objetive, int rangeBomb){
    return (abs(pos.x - objetive.x) <= rangeBomb && pos.y == objetive.y) || ((abs(pos.y - objetive.y) <= rangeBomb) &&  pos.x == objetive.x);
}




sf::Vector2i generateObjetive2Player(std::shared_ptr<Entity> p, std::shared_ptr<Entity> e,  std::vector<sf::Vector2i> &objetives);

void seekAnyPlayerOrRandom(list<ANode_Ptr> & movements, std::shared_ptr<Entity> e,  TypeSeekIA typeSeek);

inline void createRangeVision(sf::Vector2i &from, sf::Vector2i &to, sf::Vector2i position, int rangeVision);


//#include "../Include/EntitiesInclude.hpp"
#include "../Entities/Player.h"
void selectPowerUpsObjetive(std::shared_ptr<Entity> e, std::vector<sf::Vector2i> &objetives, int rangeVision);
bool checkValidPositionWithImprudence(const sf::Vector2i &v, std::shared_ptr<Entity> e,int CostPath, int & incrementCost);
void generateOmitedZoneByBomb(sf::Vector2i bombPosition, std::list<OmittedArea> &AreasOmited, float timeBomb);
void generateOmitedZones(sf::Vector2i positionP, std::list<OmittedArea> &AreasOmited, int rangeVision);
void selectEnemyPlayers(std::shared_ptr<Entity> IA, std::vector<sf::Vector2i> &objetives, int rangeVision);
void tryKillAEnemy(std::shared_ptr<Entity> IA, std::list<ANode_Ptr> &movements, int rangeVision, int costDestroy);
bool somePlayerEnemyOnVision(sf::Vector2i pos, int rangeVision, int team);
bool somePlayerEnemyOnRange(sf::Vector2i pos, int rangeBomb, int team);
bool somePlayerEnemyOnRangeThrow(sf::Vector2i pos, int rangeBomb, int team, LookingAt &lAt);
bool somePlayerEnemyOnRangeKick(sf::Vector2i pos, int rangeBomb, int team, LookingAt &lAt);
bool canPutABombSafe(sf::Vector2i posBomb, std::shared_ptr<PlayerEntity> e, std::list<ANode_Ptr> &movements);
void go2SafeZone(sf::Vector2i posBomb, std::shared_ptr<PlayerEntity> e, std::list<ANode_Ptr> &movements);
bool canThrowBombSafe(sf::Vector2i currentPosition, int rangeBomb, const LookingAt &l);
sf::Vector2i  getFallBomb(sf::Vector2i currentPosition, int rangeBomb, const LookingAt &l);
bool canKickBombSafe(sf::Vector2i currentPosition, int rangeBomb, const LookingAt &l);
Interst_ptr generateIntersetPointDestroyer(sf::Vector2i posPossibleBom);
bool pathFinderDestroy2Farm(const sf::Vector2i &positionEnemy, std::list<ANode_Ptr> &path, std::shared_ptr<Entity>  e, int costAddDestroy);
bool pathFindingGoWithCare(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> &path, std::shared_ptr<Entity>  e, int costAddDestroy);
bool pathFindingGoSafeArea(const sf::Vector2i &positionEnemy, std::list<ANode_Ptr> &path, std::shared_ptr<Entity>  e, int costAddDestroy);
bool checkIfWillDestroy(sf::Vector2i wallPos);
bool PanicMode(sf::Vector2i cPosition, ActionsAvalible aa, LookingAt &at);
bool PanicMode(sf::Vector2i posPLayer, ActionsAvalible aa, LookingAt &at, std::list<ANode_Ptr> &movements);
#include "OmittedAreas.hpp"

inline void addThisZone2OmittedArea(sf::Vector2i cPos, std::list<OmittedArea> & oaList, int powerOfBombs ){
    for(int x = -powerOfBombs; x < powerOfBombs+1; x++){
        oaList.push_back(OmittedArea(sf::Vector2i(cPos.x + x, cPos.y)));
    }
    for(int y = -powerOfBombs; y < powerOfBombs+1; y++){
        oaList.push_back(OmittedArea(sf::Vector2i(cPos.x, cPos.y+ y)));
    }
}

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