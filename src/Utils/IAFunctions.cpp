#include "IAFunctions.hpp"
#include "../Map/Level.hpp"
#include "../Include/EntitiesInclude.hpp"
#include "../PseudoPPDL/Estados.hpp"

//Map Intersting Destroy
std::vector<std::vector<Interst_ptr>> PointsDestroyMap::interestingMap;
Interst_ptr &PointsDestroyMap::getIntersetZone(sf::Vector2i pos){
    return getIntersetZone(pos.x, pos.y);
}

void PointsDestroyMap::addInterset(sf::Vector2i pos, Interst_ptr in){
    addInterset(pos.x, pos.y, in);
}

void PointsDestroyMap::addInterset(int x, int y, Interst_ptr in){
    interestingMap[y][x] = in;

}
Interst_ptr &PointsDestroyMap::getIntersetZone(int x, int y){
    return interestingMap[y][x];
}


void PointsDestroyMap::resetMap(){
    for(std::vector<Interst_ptr> inter : interestingMap){
        for(Interst_ptr i : inter){
            i = nullptr;
        }
        inter.clear();
    }
    interestingMap.clear();
    sf::Vector2i sLevel =  Level::sizeLevel();
    interestingMap = std::vector<std::vector<Interst_ptr>>(sLevel.y, std::vector<Interst_ptr>(sLevel.x, nullptr));
}



void PointsDestroyMap::updateMap(){
    resetMap();
    sf::Vector2i sLevel =  Level::sizeLevel();
    for(int x = 0; x < sLevel.x; x++){
        for(int y = 0; y < sLevel.y; y++){
            Entity_ptr e = Level::getCellMiniMapObject(x,y);
            if(e == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr){
                Interst_ptr i = generateIntersetPointDestroyer(sf::Vector2i(x,y)); 
                if(i->intersest() > 0){
                    addInterset(x,y, i);
                }else{
                    addInterset(x,y, nullptr);
                }
            }else{
                addInterset(x,y, nullptr);
            }
        }
    }
}


/************************
 * 
 * IA
 ************************/ 

inline void createRangeVision(sf::Vector2i &from, sf::Vector2i &to, sf::Vector2i position, int rangeVision){
        to = Level::sizeLevel() - sf::Vector2i(1,1);
        from = sf::Vector2i(1,1); //ignorar bordes
    if(rangeVision > -1){
        sf::Vector2i aux = position - sf::Vector2i(rangeVision, rangeVision);
        from.x = max(from.x, aux.x);
        from.y = max(from.y, aux.y);
        aux = position + sf::Vector2i(rangeVision, rangeVision);
        to.x = min(to.x, aux.x);
        to.y = min(to.y, aux.y);
    }
}

sf::Vector2i generateObjetive2Player(Entity_ptr p, Entity_ptr e, std::vector<sf::Vector2i> &objetives)
{
    sf::Vector2f posPlayer = p->getCenterPosition();
    sf::Vector2i posMapNeeded = getMapCoordinates(posPlayer);
    switch (e->typePosIA)
    {
    case TypePositionRelative::OBJETIVE_POSITION:
        objetives.push_back(posMapNeeded);
        //std::cout << posMapNeeded.x << " " << posMapNeeded.y << "\n";
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
            if (!p->dead && !p->respawning && isOnVision(p, mapPosition, e->rangoVision))
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

bool checkValidPositionWithImprudence(const sf::Vector2i &v, std::shared_ptr<Entity> e, int CostPath, int & incrementCost){
    PlayerIA_ptr p = std::dynamic_pointer_cast<PlayerIAEntity>(e);
    incrementCost = 0;
    bool valid = (Level::isValidCell(v) && (Level::getCellMiniMapObject(v) == nullptr || !Level::getCellMiniMapObject(v)->isColliderWith(e)));
    if(valid){
        //Verficar si es un area omitida
        if(e->OmittedAreas.size() > 0){
            for(OmittedArea oa : e->OmittedAreas){
                if(oa == v){
                    if(abs((CostPath * SIZE_PILLAR/ (p->baseSpeed * p->speedBoost) ) - oa.TimeAp()) < 3 /* || p->avanzaAtravesDelFuego(abs(CostPath - oa.TimeAp())) */){
                        valid = false;
                        break;
                    }else{
                        incrementCost = std::dynamic_pointer_cast<PlayerIAEntity>(e)->sg._KillStruct.imprudencia;
                    }
                }//cost* sizePilar/ baseSpeed * speedBoost * 
            }
        }
    }
    return valid;
}


//////////////////////////////////
////  Interst/Omited Zones   /////
//////////////////////////////////

// timeToPutNewBomb < 0 -> no quiere poner bomba
bool WillDead(sf::Vector2i posPLayer, float timeToPutNewBomb){
    sf::Vector2i sLevel = Level::sizeLevel();
    bool willDead = false;
    for(int x = 1; !willDead && x < sLevel.x - 1; x++){
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(x, posPLayer.y));
        if(b != nullptr && (timeToPutNewBomb < 0 || timeToPutNewBomb > b->getExplosionTimeLeft())){
            willDead = b->bombPower >= abs(x - posPLayer.x);
        }
    }
    if(willDead){
        return true;
    }

    for(int y = 1; !willDead && y < sLevel.y - 1; y++){
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(posPLayer.x, y));
        if(b != nullptr && (timeToPutNewBomb < 0 || timeToPutNewBomb > b->getExplosionTimeLeft())){
            willDead = b->bombPower >= abs(y - posPLayer.y);
        }
    }
    return willDead;
}

int getFiresOnPlayer(sf::Vector2i posPLayer, float time2Pass){
    sf::Vector2i sLevel = Level::sizeLevel();
    int countFires = 0;
    for(int x = 1; x < sLevel.x - 1; x++){
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(x, posPLayer.y));
        if(b != nullptr && ( abs(time2Pass - b->getExplosionTimeLeft()) < 10)){
            if(b->bombPower >= abs(x - posPLayer.x)){
                countFires++;
            }
        }
    }

    for(int y = 1; y < sLevel.y - 1; y++){
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(posPLayer.x, y));
        if(b != nullptr && ( abs(time2Pass - b->getExplosionTimeLeft()) < 10)){
            if(b->bombPower >= abs(y - posPLayer.y)){
                countFires++;
            }
        }
    }
    return countFires;
}

void generateOmitedZoneByBomb(sf::Vector2i bombPosition, std::list<OmittedArea> &AreasOmited, float timeBomb, int powerBomb)
{
    //TODO: Posiblidad de cruz más grande?
    for(int i = -powerBomb; i <= powerBomb; i++){
        OmittedArea o1(bombPosition + sf::Vector2i(i, 0), timeBomb);
        OmittedArea o2(bombPosition + sf::Vector2i(0, i), timeBomb);
        AreasOmited.push_back(o1);
        AreasOmited.push_back(o2);

    }

    //Añadir posicion de bomba?? no debería ser necesario por imposibilidad de llegar
}

void generateOmitedZones(sf::Vector2i positionP, std::list<OmittedArea> &AreasOmited, int rangeVision)
{
    sf::Vector2i by;
    sf::Vector2i to;
    createRangeVision(by, to, positionP, rangeVision);
    for (int x = by.x; x < to.x; x++)
    {
        for (int y = by.y; y < to.y; y++)
        {
            Entity_ptr e = Level::getCellMiniMapObject(x, y);
            Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(e);
            if ( b != nullptr)
            {
                generateOmitedZoneByBomb(sf::Vector2i(x, y), AreasOmited, b->getExplosionTimeLeft(), b->bombPower);
            }else if(std::dynamic_pointer_cast<Fire>(e) != nullptr){
                //
                generateOmitedZoneByBomb(sf::Vector2i(x, y), AreasOmited, 0, 1);
            }
        }
    }
}



Interst_ptr generateIntersetPowerUps(Entity_ptr e, PlayerIA_ptr p){
    if(std::dynamic_pointer_cast<BuffPoweUp>(e) != nullptr){
        return std::make_shared<IntersetArea>(e->getEntityMapCoordinates(),p->getIntersetBuffPE());
    }
    if(std::dynamic_pointer_cast<NerfPoweUp>(e) != nullptr){
        return std::make_shared<IntersetArea>(e->getEntityMapCoordinates(),p->getIntersetNerfPU() );
    }
    if(std::dynamic_pointer_cast<BuffPoweUp>(e) != nullptr){
        return std::make_shared<IntersetArea>(e->getEntityMapCoordinates(),p->getIntersetActionPU());
    }
    return nullptr;
}

bool checkIsDirectAccesible(sf::Vector2i posA, sf::Vector2i posB){
    if(posA.x != posB.x && posA.y != posB.y){
        return false;
    }
    if(posA.x == posB.x && posA.y == posB.y){
        return true;
    }
    
    sf::Vector2f nDir = normalize(posB - posA);
    bool accesible = true;
    if(nDir.x != 0){
        for(int x = posA.x + nDir.x; accesible && x != (posB.x); x += nDir.x){
            Entity_ptr e = Level::getCellMiniMapObject(x, posA.y);
            accesible = e == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr;
        }
    }else{
        for(int y = posA.y + nDir.y; accesible && y != (posB.y); y += nDir.y){
            Entity_ptr e = Level::getCellMiniMapObject(posA.x, y);
            accesible = e == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr;
        }
    }
    return accesible;
}

bool checkIfWillDestroy(sf::Vector2i wallPos){
    bool willDestroy = false;
    for(Entity_ptr e : Level::getEntities2Update() ){
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(e);
        if(b != nullptr){
            sf::Vector2i bombPos = b->getEntityMapCoordinates();
            if(b->onFlight){
                bombPos = getMapCoordinates(b->getObjetive());
            }
            if(isOnRangeBomb(bombPos, wallPos, b->bombPower)  && checkIsDirectAccesible(wallPos, bombPos)){
                willDestroy = true;
                break;
            }
        }
    }
    return willDestroy;
}

Interst_ptr generateIntersetPointDestroyer(sf::Vector2i posPossibleBom){
    sf::Vector2i sLevel = Level::sizeLevel();
    Entity_ptr m;
    int sum = 0;
    if( !(posPossibleBom.x > -1 && posPossibleBom.y > -1 && posPossibleBom.x < sLevel.x && posPossibleBom.y < sLevel.y) ){
        return nullptr;
    }
    
    ///Oeste
    if(posPossibleBom.x > 0){
        m = Level::getCellMiniMapObject(posPossibleBom.x-1, posPossibleBom.y);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr && !m->getExpiredEntity() 
            && !checkIfWillDestroy(m->getEntityMapCoordinates())){ sum++;}
    }
    ///Norte
    if(posPossibleBom.y > 0  ){
        m = Level::getCellMiniMapObject(posPossibleBom.x, posPossibleBom.y - 1);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr && !m->getExpiredEntity() 
            && !checkIfWillDestroy(m->getEntityMapCoordinates())){sum++;}
    }

    ///Este
    if(posPossibleBom.x < sLevel.x -1 ){
        m = Level::getCellMiniMapObject(posPossibleBom.x + 1, posPossibleBom.y);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr 
            && !m->getExpiredEntity() && !checkIfWillDestroy(m->getEntityMapCoordinates())){sum++;}
    }
    //Norte
    if(posPossibleBom.y < sLevel.y -1 ){
        m = Level::getCellMiniMapObject(posPossibleBom.x ,posPossibleBom.y  + 1);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr && !m->getExpiredEntity() 
            && !checkIfWillDestroy(m->getEntityMapCoordinates())){sum++;}
    }
    return std::make_shared<IntersetArea>(IntersetArea(posPossibleBom, sum));
}

/* void seekBetterListPos2DestroyWalls(sf::Vector2i positionCenter, int range, std::list<sf::Vector2i> &positions, PlayerIA_ptr p ){
    sf::Vector2i by;
    sf::Vector2i to;
    createRangeVision(by, to, positionCenter, range);

    int bestN = 0;
    for(int x = by.x + 1; x < to.x - 1; x++){
        for(int y = by.y + 1; y < to.y - 1; y++){
            Interst_ptr interset = generateIntersetPointDestroyer(sf::Vector2i(x,y), p);
            
            if(interset->intersest() > bestN){    //Si es 4 -> no existe camino!
                positions.clear();
                bestN = interset->intersest();
            }
            if(interset->intersest() == bestN){
                positions.push_back(sf::Vector2i(x,y));
            }
        }
    }

} */


void createStateMap(Entity_ptr e, int rangeVision,std::list<Entity_ptr>& stateMap){
    sf::Vector2i position = e->getEntityMapCoordinates();
    sf::Vector2i by;
    sf::Vector2i to;
    createRangeVision(by, to, position, e->rangoVision);

    for(int x = by.x; x < to.x; x++){
        for(int y = by.y; y < to.y; y++){
            Entity_ptr m = Level::getCellMiniMapObject(x,y);
            if(m != nullptr && std::dynamic_pointer_cast<Pillar>(m) == nullptr && std::dynamic_pointer_cast<BrickWall>(m) == nullptr){
                stateMap.push_back(m);
            }
        }
    }
}

//TODO: Criterios de seleccion?
void addPowerUpObjetive(sf::Vector2i poistionPU, std::vector<sf::Vector2i> &objetives)
{
    objetives.push_back(poistionPU);
}


void selectEnemyPlayers(Entity_ptr IA, std::vector<sf::Vector2i> &objetives, int rangeVision)
{
    sf::Vector2i positionIA = IA->getEntityMapCoordinates();
    int i = 0;
    for(Player_ptr p : PLayers::getVectorPlayer()){
        if(p->team != IA->team && !p->respawning && !p->dead){
            if(isOnVision(positionIA, p->getEntityMapCoordinates(), rangeVision)){
                objetives.push_back(p->getEntityMapCoordinates());
            }
        }
    }
}

void selectPowerUpsObjetive(Entity_ptr IA, std::vector<sf::Vector2i> &objetives, int rangeVision){
    sf::Vector2i from;
    sf::Vector2i to;
    createRangeVision(from, to, IA->getEntityMapCoordinates(), rangeVision);
    for(int x = from.x; x < to.x; x++){
        for(int y = from.y; y < to.y; y++){
            PowerUp_ptr pu = std::dynamic_pointer_cast<PowerUp>(Level::getCellMiniMapObject(x,y));
            if(pu != nullptr){
                objetives.push_back(pu->getEntityMapCoordinates());
            }
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
        if(pathFindingGoWithCare(getMapCoordinates(IA->getCenterPosition()), objetives, movements, IA,0)){
            
        }
    }
}

bool somePlayerEnemyOnVision(sf::Vector2i pos, int rangeVision, int team){
   bool onRange = false;
    for(Player_ptr p : PLayers::getVectorPlayer()){
        if(p->team != team && !p->respawning && !p->dead){
            onRange = isOnVision(pos, getMapCoordinates(p->getCenterPosition()), rangeVision);
            if(onRange){break;}
        }
    }
    return onRange;
}

bool somePlayerEnemyOnRange(sf::Vector2i pos, int rangeBomb, int team){
    bool onRange = false;
    for(Player_ptr p : PLayers::getVectorPlayer()){
        if(p->team != team && !p->respawning && !p->dead){
            onRange = isOnRangeBomb(pos, getMapCoordinates(p->getCenterPosition()), rangeBomb);
            if(onRange){break;}
        }
    }
    return onRange;
}

bool somePlayerEnemyOnRangeThrow(sf::Vector2i pos, int rangeBomb, int team, LookingAt &lAt){
    bool onRange = false;
    sf::Vector2i bomPos = getFallBomb(pos, rangeBomb, LookingAt::down);
    lAt = LookingAt::down;
    onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    
    if(!onRange){
        lAt = LookingAt::up;
        bomPos = getFallBomb(pos, rangeBomb, LookingAt::up);
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    }
    if(!onRange){
        lAt = LookingAt::left;
        bomPos = getFallBomb(pos, rangeBomb, LookingAt::left);
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    }
    if(!onRange){
        lAt = LookingAt::right;
        bomPos = getFallBomb(pos, rangeBomb, LookingAt::right);
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    }
    return onRange;
}

bool somePlayerEnemyOnRangeKick(sf::Vector2i pos, int rangeBomb, int team,  LookingAt &lAt){
    bool onRange = false;
    sf::Vector2i bomPos;
    
    if(canKickBombSafe(bomPos, rangeBomb, LookingAt::down)){
        lAt = LookingAt::down;
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    }
    if(!onRange && canKickBombSafe(bomPos, rangeBomb, LookingAt::up)){
        lAt = LookingAt::up;
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    }
    if(!onRange && canKickBombSafe(bomPos, rangeBomb, LookingAt::left)){
        lAt = LookingAt::left;
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
    }
    if(!onRange && canKickBombSafe(bomPos, rangeBomb, LookingAt::right)){
        lAt = LookingAt::right;
        onRange = somePlayerEnemyOnRange(bomPos, rangeBomb, team);
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


void go2SafeZone(sf::Vector2i posBomb, Player_ptr e, std::list<ANode_Ptr> &movements){
    std::vector<sf::Vector2i> safeCelss;
    sf::Vector2i sLevel = Level::sizeLevel();
    for(int x = 1; x < sLevel.x - 1; x++){
        for(int y = 1; y < sLevel.y; y++){
            Entity_ptr eM = Level::getCellMiniMapObject(x,y);
            if(eM == nullptr || std::dynamic_pointer_cast<PowerUp>(eM) ){
                bool safe = true;
                sf::Vector2i cPos(x,y);
                for(OmittedArea & oa : e->OmittedAreas){
                    if(oa == cPos ){
                        safe = false;
                        break;
                    }
                }
                if(safe){
                    safeCelss.push_back(sf::Vector2i(x,y));
                }
            }
        }
    }
    std::list<ANode_Ptr> movementsAux;
    if(pathFindingGoWithCare(getMapCoordinates(e->getCenterPosition()), safeCelss, movementsAux, e, 0)){
        movements = movementsAux;
    }
}


bool canPutABombSafe(sf::Vector2i posBomb, Player_ptr e, std::list<ANode_Ptr> &movements)
{
    int range = e->getPowerOfBombs();
    //std::cout << "pos bomb " << posBomb.x << " " << posBomb.y;
    std::vector<sf::Vector2i> objetives;
    for (int i = -(range + 2); i <= range + 2; i++)
    {
        for (int j = -(range + 2); j <= range + 2; j++)
        {
            if(i != 0 && j != 0){
                sf::Vector2i pos =  posBomb + sf::Vector2i(i,j);
                //std:: cout << " Los 2 distintos ";
                if(checkValidPosition(pos, e)){
                    objetives.push_back(pos);
                  //  std:: cout << " Y añadidos\n";
                }
            }else if((j == 0  &&  abs(i) > range+1 ) || ( i == 0 && abs(j) > range +1 )){
                sf::Vector2i pos =  posBomb + sf::Vector2i(i,j);
                //std::cout << "pos Fire " << pos.x << " " << pos.y << "\n";
                if(Level::isValidCell(pos) && (Level::getCellMiniMapObject(pos) == nullptr || !Level::getCellMiniMapObject(pos)->isColliderWith(e))){
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

sf::Vector2i  getFallBomb(sf::Vector2i currentPosition, int rangeBomb, const LookingAt &l){
    sf::Vector2i fallPosition(currentPosition.x,currentPosition.y);
    sf::Vector2i desplaceOnCollision(0, 0);
    switch (l)
    {
    case LookingAt::down:
        fallPosition.y += 5;
        desplaceOnCollision.y = 1;
        break;
    case LookingAt::up:
        fallPosition.y -= 5;
        desplaceOnCollision.y = -1;
        break;
    case LookingAt::left:
        fallPosition.x -= 5;
        desplaceOnCollision.x = -1;
        break;
    case LookingAt::right:
        fallPosition.x += 5;
        desplaceOnCollision.x = 1;
        break;

    default:
        break;
    }
    sf::Vector2i sLevel = Level::sizeLevel();
    fallPosition.x = max(min(fallPosition.x, sLevel.x-1), 0);
    fallPosition.y = max(min(fallPosition.y, sLevel.y-1), 0);
    Entity_ptr e = Level::getCellMiniMapObject(fallPosition);
    while (e != nullptr && std::dynamic_pointer_cast<PowerUp>(e) == nullptr)
    {
        fallPosition = fallPosition + desplaceOnCollision;
        if(!Level::isValidCell(fallPosition))   { //Se ha salido -> rebotar
            desplaceOnCollision = -desplaceOnCollision;
            fallPosition = fallPosition + desplaceOnCollision; //recuperar posicion anterior ya determinada como colision
            fallPosition = fallPosition + desplaceOnCollision;
        }
        e = Level::getCellMiniMapObject(fallPosition);
    }
    return fallPosition;
}


bool canThrowBombSafe(sf::Vector2i currentPosition, int rangeBomb, const LookingAt &l){
    sf::Vector2i fallPosition(currentPosition.x,currentPosition.y);
    sf::Vector2i desplaceOnCollision(0, 0);
    switch (l)
    {
    case LookingAt::down:
        fallPosition.y += 5;
        desplaceOnCollision.y = 1;
        break;
    case LookingAt::up:
        fallPosition.y -= 5;
        desplaceOnCollision.y = -1;
        break;
    case LookingAt::left:
        fallPosition.x -= 5;
        desplaceOnCollision.x = -1;
        break;
    case LookingAt::right:
        fallPosition.x += 5;
        desplaceOnCollision.x = 1;
        break;

    default:
        break;
    }
    sf::Vector2i sLevel = Level::sizeLevel();
    fallPosition.x = max(min(fallPosition.x, sLevel.x-1), 0);
    fallPosition.y = max(min(fallPosition.y, sLevel.y-1), 0);
    Entity_ptr e = Level::getCellMiniMapObject(fallPosition);
    while (e != nullptr && std::dynamic_pointer_cast<PowerUp>(e) == nullptr)
    {
        fallPosition = fallPosition + desplaceOnCollision;
        if(!Level::isValidCell(fallPosition))   { //Se ha salido -> rebotar
            desplaceOnCollision = -desplaceOnCollision;
            fallPosition = fallPosition + desplaceOnCollision; //recuperar posicion anterior ya determinada como colision
            fallPosition = fallPosition + desplaceOnCollision;
        }
        e = Level::getCellMiniMapObject(fallPosition);
    }
    
    return abs(currentPosition.x - fallPosition.x)  > 1 || abs(currentPosition.y - fallPosition.y)  > 1 ;
}


bool canKickBombSafe(sf::Vector2i currentPosition, int rangeBomb, const LookingAt &l){
    sf::Vector2i siteCollision(currentPosition.x,currentPosition.y);
    sf::Vector2i desplaceOnCollision(0, 0);
    switch (l)
    {
    case LookingAt::down:
        desplaceOnCollision.y = 1;
        break;
    case LookingAt::up:
        desplaceOnCollision.y = -1;
        break;
    case LookingAt::left:
        desplaceOnCollision.x = -1;
        break;
    case LookingAt::right:
        desplaceOnCollision.x = 1;
        break;

    default:
        break;
    }
    sf::Vector2i sLevel = Level::sizeLevel();
    
    Entity_ptr e;
    bool outMap = !Level::isValidCell(siteCollision);
    if(outMap){
        return false;
    }

    while (!outMap && e == nullptr || std::dynamic_pointer_cast<PowerUp>(e) != nullptr)
    {
        siteCollision = siteCollision + desplaceOnCollision;
        outMap =  !Level::isValidCell(siteCollision);
        if(!outMap){
            e = Level::getCellMiniMapObject(siteCollision);
        }
        
    }
    
    return !outMap && abs(currentPosition.x - siteCollision.x)  > rangeBomb || abs(currentPosition.y - siteCollision.y)  > rangeBomb;
}

//Go To Objetive
// GoToObjetive(x, y)
// Pre: onVision(y) ^ existPath(x, y) ^ onPostion(IA, x) ^ ¬onPosition(IA, y)
// B: onPosition(IA, x)
// A: onPosition(IA, y)



#include "Action.hpp"


void ss(sf::Vector2i p ){
    bool valid = Level::isValidCell(p);
    if(!valid){
        return;
    }
    Entity_ptr e = Level::getCellMiniMapObject(p);
    Interst_ptr inetersDest = PointsDestroyMap::getIntersetZone(p);
    float intDestroy = 0;
    if(inetersDest != nullptr){intDestroy = inetersDest->intersest();}
    if(std::dynamic_pointer_cast<BuffPoweUp>(e)){
        
    }
}

bool pathFinderDestroy2Farm(const sf::Vector2i &positionEnemy, std::list<ANode_Ptr> &path, Entity_ptr e, int costAddDestroy)
{
    PlayerIA_ptr IA =  std::dynamic_pointer_cast<PlayerIAEntity>(e);
    path.clear();
    Heap<ANode_Ptr> frontera;
    std::map<vec2i, ANode_Ptr> expanded;
    std::map<vec2i, int> objetivesFound;
    std::cout << positionEnemy.x << " --- " << positionEnemy.y << std::endl;

    bool haveInterset = IA->getIntersetDestroyWalls() > 0;

    Interst_ptr levelInterset = PointsDestroyMap::getIntersetZone(positionEnemy);
    int interestSite = levelInterset != nullptr ? levelInterset->intersest() : 0;
    int firstIntersetSite = interestSite;
    int bestfounds = interestSite;
    auto currentNode = std::make_shared<ANode>(ANode(0, positionEnemy, interestSite));
    ANode_Ptr lastBest = currentNode;
    if(interestSite == 3){
        path.push_back(currentNode);
        return true;
    }

    bool found = false;
    while (!found)
    {

        expanded[vec2i(currentNode->getPosition())] = currentNode;
        //expandir nodos
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (abs(i) != abs(j))
                {
                    sf::Vector2i nodePosition(currentNode->xPosition() + i, currentNode->yPosition() + j);
                    
                    if(!Level::isValidCell(nodePosition)){
                        continue;
                    }
                    levelInterset = PointsDestroyMap::getIntersetZone(nodePosition);
                    interestSite = levelInterset != nullptr ? levelInterset->intersest() : 0;
                    if (levelInterset > 0) {
                        std::cout << levelInterset << std::endl;
                    }
                    ANode_Ptr newNode = std::make_shared<ANode>(ANode(currentNode->fAcum() + 1, nodePosition, interestSite ,currentNode));
                    int incrementCost = 0;
                    if (checkValidPositionWithImprudence(nodePosition, e, newNode->costNode(), incrementCost) && isOnVision(nodePosition, positionEnemy, IA->sg._SeekPEStruct.RangoVision)
                             && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(newNode))
                    { //Si es una posicion valida y no se ha expandido
                        newNode->incrementCost(incrementCost); // TODO: variable segun IA
                        frontera.add(newNode);
                        if( bestfounds < interestSite){
                            bestfounds = interestSite;
                            lastBest = newNode;
                        }
                    }
                    else
                    {
                        newNode = nullptr;
                    }
                }
            }
        }
        //Extraer nodo
        if (frontera.isEmpty())
        {
            break;
        }
        currentNode = frontera.pop();

        //currentNode = *
        if (currentNode->isObjetive())
        {
            lastBest = currentNode;
            found =  true; 
        }
    }

    std::list<ANode_Ptr> list_actions;
    //Si no se ha encontrado -> seleccionar aleatorio
    if (!found && lastBest == nullptr)
    {
        return false;
    }
    found = lastBest != nullptr;
    if(lastBest->getParent() == nullptr){
        //the best is the current site
        if(firstIntersetSite != 0){
            path.push_back(lastBest);
            return true;
        }else{
            return false;
        }
    }

    while (lastBest != nullptr)
    {
        if (lastBest->getParent() != nullptr)
        {
            list_actions.push_back(lastBest);
        }
        lastBest = lastBest->getParent();
    }

    while (!list_actions.empty())
    {
        ANode_Ptr e = list_actions.back();
        path.push_back(e);
        list_actions.pop_back();
    }
    return found;
}



bool pathFinderActions( Entity_ptr _ia, const std::vector<sf::Vector2i> &objetives, std::list<Action_ptr> &actions, TypeSeekIA typeSeek, int costAddDestroy){
    sf::Vector2i postionIA = _ia->getEntityMapCoordinates();
    PlayerIA_ptr IA = std::dynamic_pointer_cast<PlayerIAEntity>(_ia);
    actions.clear();
    Heap<NodeAction_ptr> frontera;
    std::map<vec2i, NodeAction_ptr> expanded;
    std::map<vec2i, int> objetivesFound;
    for(sf::Vector2i o: objetives){
        objetivesFound[vec2i(o)] = 0;
    }
    sf::Vector2i objetive = selectCloseObjetive(postionIA, objetives);
    
    int bestfounds = 0;
    NodeAction_ptr lastBest;
    NodeAction_ptr currentNode = std::make_shared<NodeAction>(NodeAction(postionIA, objetive, 0.0f));

    bool found = false;
    while (!found)
    {

        expanded[vec2i(currentNode->getPosition())] = currentNode;
        //expandir nodos
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (abs(i) != abs(j))
                {
                    sf::Vector2i nodePosition(currentNode->xPosition() + i, currentNode->yPosition() + j);
                    sf::Vector2i objetiveP = selectCloseObjetive(nodePosition, objetives);
                    NodeAction_ptr newNode = std::make_shared<NodeAction>(NodeAction(nodePosition, objetiveP, currentNode->fAcum() + 1, currentNode));
                    if (/* checkValidPositionOrDestroyer(nodePosition, _ia) &&  */expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(currentNode))
                    { //Si es una posicion valida y no se ha expandido
                        newNode->incrementCost(costAddDestroy); // TODO: variable segun IA
                        frontera.add(newNode);
                    }
                    else
                    {
                        newNode = nullptr;
                    }
                }
            }
        }
        //Extraer nodo
        if (frontera.isEmpty())
        {
            break;
        }
        currentNode = frontera.pop();

        //currentNode = *
        if (currentNode->isObjetive())
        {
            lastBest = currentNode;
            sf::Vector2i posObjetiv =  currentNode->getPosition();
            vec2i pOb = vec2i(posObjetiv);
            objetivesFound[pOb]++;
            found = typeSeek == TypeSeekIA::BEST_PATH || (objetivesFound[pOb] > 0 && typeSeek == TypeSeekIA::SECOND_BEST_PATH) || (typeSeek == TypeSeekIA::LONG_PATH && int(manhattan(posObjetiv, postionIA) * 1.5) <= currentNode->costNode());
          //  std::cout << "Coste camino" << currentNode->costNode() << "\n";
            bestfounds++;
            if(frontera.isEmpty()){
                break;
            }
            if(!found){
                currentNode = frontera.pop();
            }
        }
    }

    return false;
}

bool pathFindingGoWithCare(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> &path, Entity_ptr e, int costAddDestroy)
{
    
    PlayerIA_ptr IA =  std::dynamic_pointer_cast<PlayerIAEntity>(e);
    path.clear();
    Heap<ANode_Ptr> frontera;
    std::map<vec2i, ANode_Ptr> expanded;
    std::map<vec2i, int> objetivesFound;

    bool haveInterset = IA->getIntersetDestroyWalls() > 0;
    
    ANode_Ptr lastBest;
    Interst_ptr levelInterset;
    int interestSite = levelInterset != nullptr ? levelInterset->intersest() : 0;
    int bestfounds = interestSite;
    sf::Vector2i objetiveP = selectCloseObjetive(positionEnemy, objetives);
    ANode_Ptr currentNode = std::make_shared<ANode>(ANode(positionEnemy, sf::Vector2i(0, 0), objetiveP, 0));

    bool found = false;
    while (!found)
    {

        expanded[vec2i(currentNode->getPosition())] = currentNode;
        //expandir nodos
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (abs(i) != abs(j))
                {
                    sf::Vector2i nodePosition(currentNode->xPosition() + i, currentNode->yPosition() + j);
                    if(!Level::isValidCell(nodePosition)){
                        continue;
                    }
                    sf::Vector2i objetiveP = selectCloseObjetive(nodePosition, objetives);
                    ANode_Ptr newNode = std::make_shared<ANode>(ANode(nodePosition, sf::Vector2i(i, j), objetiveP, currentNode->fAcum() + 1, currentNode));//std::make_shared<ANode>(ANode(nodePosition, currentNode->fAcum() + 1, interestSite ,currentNode));
                    int incrementCost = 0;
                    if (checkValidPositionWithImprudence(nodePosition, e, newNode->costNode(), incrementCost) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(newNode))
                    { //Si es una posicion valida y no se ha expandido
                        newNode->incrementCost(incrementCost);
                        levelInterset = generateIntersetPowerUps(Level::getCellMiniMapObject(nodePosition), IA);
                        if(levelInterset != nullptr){
                            newNode->incrementCost(levelInterset->intersest()); // TODO: variable segun IA
                        }
                        frontera.add(newNode);
                    }
                    else
                    {
                        newNode = nullptr;
                    }
                }
            }
        }
        //Extraer nodo
        if (frontera.isEmpty())
        {
            break;
        }
        currentNode = frontera.pop();

        //currentNode = *
        if (currentNode->isObjetive())
        {
            lastBest = currentNode;
            found =  true; 
        }
    }

    std::list<ANode_Ptr> list_actions;
    //Si no se ha encontrado -> seleccionar aleatorio
    if (!found && lastBest == nullptr)
    {
        return false;
    }
    found = lastBest != nullptr;
    while (lastBest != nullptr)
    {
        if (lastBest->getParent() != nullptr)
        {
            list_actions.push_back(lastBest);
        }
        lastBest = lastBest->getParent();
    }

    while (!list_actions.empty())
    {
        ANode_Ptr e = list_actions.back();
        path.push_back(e);
        list_actions.pop_back();
    }
    return found;
}



bool pathFindingGoSafeArea(const sf::Vector2i &positionEnemy, std::list<ANode_Ptr> &path, Entity_ptr e, int costAddDestroy)
{
    std::cout << "Go safe Area\n";
    PlayerIA_ptr IA =  std::dynamic_pointer_cast<PlayerIAEntity>(e);
    path.clear();
    Heap<ANode_Ptr> frontera;
    std::map<vec2i, ANode_Ptr> expanded;
    std::map<vec2i, int> objetivesFound;

    bool haveInterset = IA->getIntersetDestroyWalls() > 0;
    
    ANode_Ptr lastBest;
    Interst_ptr levelInterset;
    int interestSite = levelInterset != nullptr ? levelInterset->intersest() : 0;
    int bestfounds = interestSite;
    int f = getFiresOnPlayer(positionEnemy, 0);
    ANode_Ptr currentNode = std::make_shared<ANode>(ANode(positionEnemy, 0, f, true));
    if(f == 0){
        path.push_back(currentNode);
        return true;
    }

    bool found = false;
    while (!found)
    {

        expanded[vec2i(currentNode->getPosition())] = currentNode;
        //expandir nodos
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (abs(i) != abs(j))
                {
                    sf::Vector2i nodePosition(currentNode->xPosition() + i, currentNode->yPosition() + j);
                    if(!Level::isValidCell(nodePosition)){
                        continue;
                    }
                    f = getFiresOnPlayer(nodePosition, currentNode->costNode());
                    ANode_Ptr newNode = std::make_shared<ANode>(ANode(nodePosition, currentNode->costNode(), f, true, currentNode));//std::make_shared<ANode>(ANode(nodePosition, currentNode->fAcum() + 1, interestSite ,currentNode));
                    int incrementCost = 0;
                    if (checkValidPositionWithImprudence(nodePosition, e, newNode->costNode(), incrementCost) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(newNode))
                    { //Si es una posicion valida y no se ha expandido
                        newNode->incrementCost(incrementCost); // TODO: variable segun IA
                        frontera.add(newNode);
                    }
                    else
                    {
                        newNode = nullptr;
                    }
                }
            }
        }
        //Extraer nodo
        if (frontera.isEmpty())
        {
            break;
        }
        currentNode = frontera.pop();

        //currentNode = *
        if (currentNode->isObjetive())
        {
            lastBest = currentNode;
            found =  true; 
        }
    }

    std::list<ANode_Ptr> list_actions;
    //Si no se ha encontrado -> seleccionar aleatorio
    if (!found && lastBest == nullptr)
    {
        return false;
    }
    found = lastBest != nullptr;
    while (lastBest != nullptr)
    {
        if (lastBest->getParent() != nullptr)
        {
            list_actions.push_back(lastBest);
        }
        lastBest = lastBest->getParent();
    }

    while (!list_actions.empty())
    {
        ANode_Ptr e = list_actions.back();
        path.push_back(e);
        list_actions.pop_back();
    }
    return found;
}


bool canUseAbilityPM(sf::Vector2i pos, ActionsAvalible aa, sf::Vector2i nextPos){
    Entity_ptr e;
    if(!Level::isValidCell(nextPos)){
        return false;
    }
    e = Level::getCellMiniMapObject(nextPos);

    switch (aa)
    {
    case ActionsAvalible::GRAB_BOMB:
        return true;
        break;
    case ActionsAvalible::KICK_BOM:
        if(e != nullptr && std::dynamic_pointer_cast<PowerUp>(e) == nullptr){
            return false;
        }
        break;
    
    default:
        break;
    }
}

bool PanicMode(sf::Vector2i cPosition, ActionsAvalible aa, LookingAt &at){
    if(aa != ActionsAvalible::GRAB_BOMB && aa != ActionsAvalible::KICK_BOM){
        return false;
    }

    sf::Vector2i pUp(cPosition.x, cPosition.y -1);
    sf::Vector2i pD(cPosition.x, cPosition.y +1);
    sf::Vector2i pR(cPosition.x + 1, cPosition.y);
    sf::Vector2i pL(cPosition.x - 1, cPosition.y);
    bool canUSe = false;
    if(Level::isValidCell(pUp) && std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(pUp)) != nullptr){
        if(canUseAbilityPM(pUp, aa, sf::Vector2i(cPosition.x, cPosition.y - 1))){
            at = LookingAt::up;
            canUSe = true;
        }
    }

    if(!canUSe && Level::isValidCell(pD) && std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(pD)) != nullptr){
        if(canUseAbilityPM(pD, aa, sf::Vector2i(cPosition.x, cPosition.y + 1))){
            at = LookingAt::down;
            canUSe = true;
        }
    }

    if(!canUSe && Level::isValidCell(pR) && std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(pR)) != nullptr){
        if(canUseAbilityPM(pR, aa, sf::Vector2i(cPosition.x + 1, cPosition.y ))){
            at = LookingAt::right;
            canUSe = true;
        }
    }
    if(!canUSe && Level::isValidCell(pL) && std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(pL)) != nullptr){
        if(canUseAbilityPM(pL, aa, sf::Vector2i(cPosition.x - 1, cPosition.y ))){
            at = LookingAt::left;
            canUSe = true;
        }
    }
    return canUSe;
}

bool PanicMode(sf::Vector2i posPLayer, ActionsAvalible aa, LookingAt &at, std::list<ANode_Ptr> &movements){
    sf::Vector2i sLevel = Level::sizeLevel();
    bool bombX = false;
    bool bombY = false;
    int mostNearX = 0;
    int mostNearY = 0;
    for(int x = 1;  x < sLevel.x - 1; x++){
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(x, posPLayer.y));
        sf::Vector2i posBom = sf::Vector2i(x, posPLayer.y);
        if(x == posPLayer.x){
            continue;
        }
        if(b != nullptr && checkIsDirectAccesible(posPLayer, sf::Vector2i(x, posPLayer.y)) && abs(x) < abs(mostNearX)){
            sf::Vector2f dir = normalize(posBom  - posPLayer);
            sf::Vector2i iDir(dir.x, dir.y);
            if(canUseAbilityPM(posBom, aa, posBom + iDir)){
            mostNearX = x;
            bombX = true;
            }
        }
        
    }
  
    for(int y = 1;  y < sLevel.y - 1; y++){
        if(y == posPLayer.y){
            continue;
        }
        Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(posPLayer.x, y));
        sf::Vector2i posBom = sf::Vector2i(posPLayer.x, y);
       if(b != nullptr && checkIsDirectAccesible(posPLayer, posBom) && abs(y) < abs(mostNearX)){
            sf::Vector2f dir = normalize(posBom  - posPLayer);
            sf::Vector2i iDir(dir.x, dir.y);
            if(canUseAbilityPM(posBom, aa, posBom + iDir)){
                mostNearY = y;
                bombY = true;
            }
        }
    }
    sf::Vector2i bomP;
    if(bombX && bombY){
        if(abs(mostNearX) < abs( mostNearY)){
            bomP = sf::Vector2i(mostNearX, posPLayer.y);
        }else{
            bomP = sf::Vector2i(posPLayer.x, mostNearY);
        }
    }else if(bombX)
    {
        bomP = sf::Vector2i(mostNearX, posPLayer.y);
    }else if(bombY){
        bomP = sf::Vector2i(posPLayer.x, mostNearY);
    }else{
        return false;
    }
    

    sf::Vector2f nDir = normalize(bomP - posPLayer);
    sf::Vector2i inDir(nDir.x, nDir.y);
    for(sf::Vector2i ip = posPLayer + inDir; ip.x != bomP.x && ip.y != bomP.y; ip = ip + inDir){
        movements.push_back(std::make_shared<ANode>(ANode(0, ip, 0)));
    }
    if(aa == ActionsAvalible::GRAB_BOMB){
        //Lanzar bomba hacia atras
        nDir = -nDir;
    }

    if(nDir.x > 0){
        at = LookingAt::right;
        return true;
    }else if(nDir.x < 0 ){
        at = LookingAt::left;
        return true;
    }else if(nDir.y > 0){
        at = LookingAt::down;
        return true;
    }else if(nDir.y < 0){
        at = LookingAt::up;
        return true;
    }
    return false;
}