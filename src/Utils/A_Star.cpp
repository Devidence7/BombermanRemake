#include "A_Star.hpp"
#include "../Include/EntitiesInclude.hpp"


bool checkObjetive(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition)
{
    return currentP.x == objetivePosition.x && currentP.y == objetivePosition.y;
}

bool checkValidPosition(const sf::Vector2i &v, Entity_ptr e)
{
    // if (!EntityMap::isValidCell(v))
    // {
    //     std::cout << "NOT VALID " << v.x << " " << v.y << std::endl;
    // }
    bool valid = (EntityMap::isValidCell(v) && (EntityMap::getCellEntMapObject(v) == nullptr || !EntityMap::getCellEntMapObject(v)->isColliderWith(e)));
    if(valid){
        //Verficar si es un area omitida
        for(OmittedArea oa : e->OmittedAreas){
            if(oa == v){
                valid = false;
                break;
            }
        }
    }
    return valid;
}

sf::Vector2i selectCloseObjetive(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives)
{
    sf::Vector2i objetive;
    int lowManhattan = 1e8;
    for (sf::Vector2i e : objetives)
    {
        int m;
        if ((m = manhattan(positionEnemy, e)) < lowManhattan)
        {
            lowManhattan = m;
            objetive = e;
        }
    }
    return objetive;
}

ANode_Ptr generateRandomMovement(Entity_ptr e, sf::Vector2i fromPosition)
{
    sf::Vector2i direction;
    sf::Vector2i nextPostion;
    int j = 0;
    do
    {
        j++;
        int mul = Random::getIntNumberBetween(0, 1);
        if (!mul)
        {
            mul = -1;
        }
        if (Random::getIntNumberBetween(0, 1))
        {
            direction.y = mul;
            direction.x = 0;
        }
        else
        {
            direction.y = 0;
            direction.x = mul;
        }
        nextPostion = fromPosition + direction;
    } while (j < 12 && !checkValidPosition(nextPostion, e));
    return std::make_shared<ANode>(ANode(nextPostion, direction, sf::Vector2i(0, 0), 0));
}

void generateRandomPath(sf::Vector2i position, std::list<ANode_Ptr> &path, Entity_ptr e)
{
    path.clear();
    ANode_Ptr node = generateRandomMovement(e, position);
    path.push_back(node);
    for (int i = 0; i < 5; i++)
    {
        node = generateRandomMovement(e, node->getPosition());
        path.push_back(node);
    }
}

void generatePath(Entity_ptr e, std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> &path)
{
    sf::Vector2i positionEnemy = getMapCoordinates(e->getCenterPosition());
    //si ve al menos 1 -> buscar
    if (!pathFinding(positionEnemy, objetives, path, e))
    {
        std::cout << "NO ENCONTARDO\n";
        objetives.clear();
    }
    else
    {
        return;
    }
    sf::Vector2i sizeMap = EntityMap::size();
    if (objetives.size() == 0)
    {
        sf::Vector2i goTo;
        if (e->rangoVision > 0)
        {
            goTo = sf::Vector2i(Random::getIntNumberBetween(-e->rangoVision, e->rangoVision), Random::getIntNumberBetween(-e->rangoVision, e->rangoVision));
        }
        else
        {
            goTo = sf::Vector2i(Random::getIntNumberBetween(-sizeMap.x - 2, sizeMap.x - 2), Random::getIntNumberBetween(-sizeMap.y - 2, sizeMap.y - 2));
        }
        goTo = positionEnemy + goTo;
        goTo.x = min(goTo.x, 1);
        goTo.y = min(goTo.y, 1);
        goTo.x = max(goTo.x, sizeMap.x);
        goTo.y = max(goTo.y, sizeMap.y);
        objetives.push_back(goTo);
    }
    pathFinding(positionEnemy, objetives, path, e);
    if (path.empty())
    {
        generateRandomPath(positionEnemy, path, e);
    }
}



bool pathFinding(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> &path, Entity_ptr e)
{
    path.clear();
    Heap<ANode_Ptr> frontera;
    std::map<vec2i, ANode_Ptr> expanded;
    std::map<vec2i, int> objetivesFound;
    for(sf::Vector2i o: objetives){
        objetivesFound[vec2i(o)] = 0;
    }
    sf::Vector2i objetive = selectCloseObjetive(positionEnemy, objetives);
    
    int bestfounds = 0;
    ANode_Ptr lastBest;
    ANode_Ptr currentNode = std::make_shared<ANode>(ANode(positionEnemy, sf::Vector2i(0, 0), objetive, 0.0f));

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
                    sf::Vector2i objetiveP = selectCloseObjetive(positionEnemy, objetives);
                    ANode_Ptr newNode = std::make_shared<ANode>(ANode(nodePosition, sf::Vector2i(i, j), objetiveP, currentNode->fAcum() + 1, currentNode));
                    if (checkValidPosition(nodePosition, e) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(currentNode))
                    { //Si es una posicion valida y no se ha expandido
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
            found = e->typeSeek == TypeSeekIA::BEST_PATH || (objetivesFound[pOb] > 0 && e->typeSeek == TypeSeekIA::SECOND_BEST_PATH) || (e->typeSeek == TypeSeekIA::LONG_PATH && int(manhattan(posObjetiv, positionEnemy) * 1.5) <= currentNode->costNode());
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

    std::list<ANode_Ptr> list_actions;
    //Si no se ha encontrado -> seleccionar aleatorio
    if (!found && lastBest == nullptr)
    {
        std::cout << "NO Best found\n";
        int randomNode = Random::getIntNumberBetween(0, expanded.size() - 1);
        for (auto &p : expanded)
        {
            if (randomNode > 0)
            {
                randomNode--;
            }
            else
            {
                lastBest = p.second;
                break;
            }
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