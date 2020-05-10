#include "IAFunctions.hpp"
#include "../Map/Level.hpp"
#include "../Include/EntitiesInclude.hpp"
#include "../PseudoPPDL/Estados.hpp"

sf::Vector2i generateObjetive2Player(Entity_ptr p, Entity_ptr e, std::vector<sf::Vector2i> &objetives)
{
    sf::Vector2f posPlayer = p->getCenterPosition();
    sf::Vector2i posMapNeeded = getMapCoordinates(posPlayer);
    switch (e->typePosIA)
    {
    case TypePositionRelative::OBJETIVE_POSITION:
        objetives.push_back(posMapNeeded);
        std::cout << posMapNeeded.x << " " << posMapNeeded.y << "\n";
        break;
    case TypePositionRelative::NEAR_POSITION:
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (i != j && i != 0)
                {
                    sf::Vector2i posObj = posMapNeeded + sf::Vector2i(i, j);
                    if (EntityMap::isValidCell(posObj))
                    {
                        objetives.push_back(posObj);
                    }
                }
            }
        }
        break;

    default:
        break;
    }

    return sf::Vector2i();
}

void seekAnyPlayerOrRandom(list<ANode_Ptr> &movements, Entity_ptr e, TypeSeekIA typeSeek)
{
    movements.clear();
    std::vector<sf::Vector2i> objetives;
    sf::Vector2i mapPosition = getMapCoordinates(e->getCenterPosition());
    if (e->rangoVision != 0)
    {

        for (Player_ptr p : PLayers::getVectorPlayer())
        {
            //if is in range
            if (isOnVision(p, mapPosition, e->rangoVision))
            {
                generateObjetive2Player(p, e, objetives);
            }
        }
        if (objetives.size() > 0)
        {
            generatePath(e, objetives, movements, typeSeek);
        }
        else
        {
        }
    }
    if (movements.size() < 1)
    {
        generateRandomPath(mapPosition, movements, e);
    }
}

/******************************
 * IA Bomberman
 * 
 ******************************/

void generateOmitedZoneByBomb(sf::Vector2i bombPosition, std::list<OmittedArea> &AreasOmited)
{
    //TODO: Posiblidad de cruz más grande?
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (i != j)
            {
                AreasOmited.push_back(bombPosition + sf::Vector2i(i, j));
            }
        }
    }
    //Añadir posicion de bomba?? no debería ser necesario por imposibilidad de llegar
}

void generateOmitedZones(std::list<OmittedArea> &AreasOmited)
{
    sf::Vector2i sizeLevel = Level::sizeLevel();
    for (int x = 0; x < sizeLevel.x; x++)
    {
        for (int y = 0; y < sizeLevel.y; y++)
        {
            Entity_ptr e = Level::getCellMiniMapObject(x, y);
            if (std::dynamic_pointer_cast<Bomb>(e) != nullptr)
            {
                generateOmitedZoneByBomb(sf::Vector2i(x, y), AreasOmited);
            }
        }
    }
}

//TODO: Criterios de seleccion?
void addPowerUpObjetive(sf::Vector2i poistionPU, std::vector<sf::Vector2i> &objetives)
{
    objetives.push_back(poistionPU);
}

sf::Vector2i seekPowerUp(list<ANode_Ptr> &movements, Entity_ptr e)
{
    std::vector<sf::Vector2i> objetives;
    sf::Vector2i sizeLevel = Level::sizeLevel();
    for (int x = 0; x < sizeLevel.x; x++)
    {
        for (int y = 0; y < sizeLevel.y; y++)
        {
            Entity_ptr e = Level::getCellMiniMapObject(x, y);
            if (std::dynamic_pointer_cast<PowerUp>(e) != nullptr)
            {
                //generateOmitedZoneByBomb(sf::Vector2i(x,y), AreasOmited);
            }
        }
    }
}

void selectEnemyPlayers(Entity_ptr IA, std::vector<sf::Vector2i> &objetives, int rangeVision)
{

    for (Player_ptr p : PLayers::getVectorPlayer())
    {
        if (IA->team != p->team && isOnVision(p, getMapCoordinates(IA->getCenterPosition()), rangeVision))
        {
            objetives.push_back(getMapCoordinates(p->getCenterPosition()));
        }
    }
}

void tryKillAEnemy(Entity_ptr IA, std::list<ANode_Ptr> &movements, int rangeVision, int costDestroy)
{
    std::vector<sf::Vector2i> objetives;
    selectEnemyPlayers(IA, objetives, rangeVision);
    if(objetives.size() < 0){
        //No hacer nada
    }else{
        if(pathFindingBreakingWalls(getMapCoordinates(IA->getCenterPosition()), objetives, movements, IA, TypeSeekIA::BEST_PATH, costDestroy)){
            
        }
    }
}



bool somePlayerEnemyOnRange(sf::Vector2i pos, int rangeBomb, int team){
    bool onRange = false;
    for(Player_ptr p : PLayers::getVectorPlayer()){
        if(p->team != team){
            onRange = isOnRange(pos, getMapCoordinates(p->getCenterPosition()), rangeBomb);
            if(onRange){break;}
        }
    }
    return onRange;
}

// //Patrol
// void Patrol(Entity_ptr e, sf::Vector2i relativeObjetive1, sf::Vector2i relativeObjetive2){
//     PlayerIA_ptr pIA = std::dynamic_pointer_cast<PlayerIAEntity>(e);
//     if(pIA != nullptr){
//         sf::Vector2i cPos = getMapCoordinates(pIA->getCenterPosition());
//         PatrolState pat(cPos + relativeObjetive1, cPos + relativeObjetive2 );
//     }
// }


//Matar

//  selectObjetive(x)
// PRE: onVision(x)
// B: --
// Add: selected(x)

//GoToObjetive(x)
// Pre: selected(x)
// B: -- 
// Add: onRange(x)



//IR a poner bomba 1
// -- onRange(x) ^ haveBomb(IA)

//IR a poner bomba 2
// -- onRange(x) ^ haveBomb(IA) ^ isBestPosition(x)


//Poner bomba
// Pre: on(e, posBomb) ^ validPosition(x) ^ canPutABombSafe(e) ^ haveBombs(e)
// Post: RunAway(IA)

bool canPutABombSafe(sf::Vector2i posBomb, Player_ptr e, std::list<ANode_Ptr> &movements)
{
    int range = e->getPowerOfBombs();
    std::vector<sf::Vector2i> objetives;
    for (int i = -(range + 1); i <= range + 1; i++)
    {
        for (int j = -(range + 1); j <= range + 1; j++)
        {
            if(abs(i) > range || abs(i) > range || i != 0 || j != 0){
                sf::Vector2i pos =  posBomb + sf::Vector2i(i,j);
                if(checkValidPosition(pos, e)){
                    objetives.push_back(pos);
                }
            }
        }
    }
    if(objetives.size() < 1){
        return false;
    }
    std::list<ANode_Ptr> movementsAux;
    if(pathFinding(getMapCoordinates(e->getCenterPosition()), objetives, movementsAux, e, TypeSeekIA::BEST_PATH, false)){
        movements = movementsAux;
        return true;
    }
    return false;
}



//Go To Objetive
// GoToObjetive(x, y)
// Pre: onVision(y) ^ existPath(x, y) ^ onPostion(IA, x) ^ ¬onPosition(IA, y)
// B: onPosition(IA, x)
// A: onPosition(IA, y)