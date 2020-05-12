#include "IAFunctions.hpp"
#include "../Map/Level.hpp"
#include "../Include/EntitiesInclude.hpp"
#include "../PseudoPPDL/Estados.hpp"

//Map Intersting Destroy
std::vector<std::vector<Interst_ptr>> PointsDestroyMap::interestingMap;
Interst_ptr &PointsDestroyMap::getIntersetZone(sf::Vector2i pos){
    return getIntersetZone(pos.x, pos.y);
}

Interst_ptr &PointsDestroyMap::addInterset(sf::Vector2i pos, Interst_ptr in){
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
    bool valid = (Level::isValidCell(v) && (Level::getCellMiniMapObject(v) == nullptr || !Level::getCellMiniMapObject(v)->isColliderWith(e)));
    if(valid){
        //Verficar si es un area omitida
        if(e->OmittedAreas.size() > 0){
            for(OmittedArea oa : e->OmittedAreas){
                if(oa == v){
                    if(abs(CostPath - oa.TimeAp()) == 0 || p->avanzaAtravesDelFuego(abs(CostPath - oa.TimeAp()))){
                        valid = false;
                        break;
                    }
                }
            }
        }
    }
    return valid;
}


//////////////////////////////////
////  Interst/Omited Zones   /////
//////////////////////////////////
void generateOmitedZoneByBomb(sf::Vector2i bombPosition, std::list<OmittedArea> &AreasOmited, float timeBomb)
{
    //TODO: Posiblidad de cruz más grande?
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (i != j)
            {
                OmittedArea o(bombPosition + sf::Vector2i(i, j), timeBomb);
                AreasOmited.push_back(o);
            }
        }
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
            Bomb_ptr b = std::dynamic_pointer_cast<Bomb>(Level::getCellMiniMapObject(x, y));
            if ( b != nullptr)
            {
                generateOmitedZoneByBomb(sf::Vector2i(x, y), AreasOmited, b->getExplosionTimeLeft());
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
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr){sum++;}
    }
    ///Norte
    if(posPossibleBom.y > 0  ){
        m = Level::getCellMiniMapObject(posPossibleBom.x, posPossibleBom.y - 1);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr){sum++;}
    }

    ///Este
    if(posPossibleBom.x < sLevel.x -1 ){
        m = Level::getCellMiniMapObject(posPossibleBom.x + 1, posPossibleBom.y);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr){sum++;}
    }
    //Norte
    if(posPossibleBom.y < sLevel.y -1 ){
        m = Level::getCellMiniMapObject(posPossibleBom.x ,posPossibleBom.y  + 1);
        if(m != nullptr && std::dynamic_pointer_cast<BrickWall>(m) != nullptr){sum++;}
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
    sf::Vector2i positionIA = IA->getEntityMapCoordinates();
    for(Player_ptr p : PLayers::getVectorPlayer()){
        if(p->team != IA->team){
            if(isOnVision(positionIA, p->getEntityMapCoordinates(), rangeVision)){
                objetives.push_back(p->getEntityMapCoordinates());
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
        if(pathFindingBreakingWalls(getMapCoordinates(IA->getCenterPosition()), objetives, movements, IA, TypeSeekIA::BEST_PATH, costDestroy)){
            
        }
    }
}

bool somePlayerEnemyOnVision(sf::Vector2i pos, int rangeVision, int team){
   bool onRange = false;
    for(Player_ptr p : PLayers::getVectorPlayer()){
        if(p->team != team){
            onRange = isOnVision(pos, getMapCoordinates(p->getCenterPosition()), rangeVision);
            if(onRange){break;}
        }
    }
    return onRange;
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
            }else if((j == 0  &&  abs(i) > range ) || ( i == 0 && abs(j) > range)){
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

    bool haveInterset = IA->getIntersetDestroyWalls() > 0;
    
    ANode_Ptr lastBest;
    Interst_ptr levelInterset = PointsDestroyMap::getIntersetZone(positionEnemy);
    int interestSite = levelInterset != nullptr ? levelInterset->intersest() : 0;
    int bestfounds = interestSite;
    ANode_Ptr currentNode = std::make_shared<ANode>(ANode(positionEnemy, 0, interestSite ,nullptr));
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
                    ANode_Ptr newNode = std::make_shared<ANode>(ANode(nodePosition, currentNode->fAcum() + 1, interestSite ,currentNode));
                    int incrementCost = 0;
                    if (checkValidPositionWithImprudence(nodePosition, e, newNode->costNode(), incrementCost) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(newNode))
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
                    if (checkValidPositionOrDestroyer(nodePosition, _ia) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(currentNode))
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
}
