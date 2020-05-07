#include "A_Star.hpp"

bool checkObjetive(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition)
{
    return currentP.x == objetivePosition.x && currentP.y == objetivePosition.y;
}

bool checkValidPosition(const sf::Vector2i &v)
{
    if (!EntityMap::isValidCell(v))
    {
        std::cout << "NOT VALID " << v.x << " " << v.y << std::endl;
    }
    return (EntityMap::isValidCell(v) && EntityMap::getCellEntMapObject(v) == nullptr);
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

ANode_Ptr generateRandomMovement(sf::Vector2i fromPosition)
{
	sf::Vector2i direction;
    sf::Vector2i nextPostion;
    int j = 0;
    do{
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
    }while(j < 12 && !checkValidPosition(nextPostion));
	return std::make_shared<ANode>(ANode(nextPostion, direction, sf::Vector2i(0, 0), 0));
}

void generateRandomPath(sf::Vector2i position, std::list<ANode_Ptr> & path)
{
    path.clear();
	ANode_Ptr node = generateRandomMovement(position);
	path.push_back(node);
	for (int i = 0; i < 5; i++)
	{
		node = generateRandomMovement(node->getPosition());
		path.push_back(node);
	}
}



void generatePath(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, int RangeVision, std::list<ANode_Ptr> & path)
{
    //si ve al menos 1 -> buscar
    std::vector<sf::Vector2i> objetivesOnRange;
    if (RangeVision != 0)
    {
        for (int i = 0; i < objetives.size(); i++)
        {
            if (RangeVision < 1 || (objetives[i].x - positionEnemy.x < RangeVision && objetives[i].y - positionEnemy.y < RangeVision))
            {
                objetivesOnRange.push_back(objetives[i]);
            }
        }

        bool finded = false;
        if(objetivesOnRange.size() > 0){
            
            if (!pathFinding(positionEnemy, objetivesOnRange, path))
            {
                objetivesOnRange.clear();
            }else{
                return;
            }
        }
        sf::Vector2i sizeMap = EntityMap::size();
        if (objetivesOnRange.size() == 0)
        {
            sf::Vector2i goTo;
            if (RangeVision > 0)
            {
                goTo = sf::Vector2i(Random::getIntNumberBetween(-RangeVision, RangeVision), Random::getIntNumberBetween(-RangeVision, RangeVision));
            }
            else
            {
                goTo = sf::Vector2i(Random::getIntNumberBetween(-sizeMap.x - 2, sizeMap.x - 2), Random::getIntNumberBetween(-sizeMap.y - 2, sizeMap.y - 2));
            }
            goTo = positionEnemy + goTo ;
            goTo.x = min(goTo.x, 1);
            goTo.y = min(goTo.y, 1);
            goTo.x = max(goTo.x, sizeMap.x);
            goTo.y = max(goTo.y, sizeMap.y);
            objetivesOnRange.push_back(goTo);
        }
        pathFinding(positionEnemy, objetivesOnRange, path);
        if(path.empty()){
            std::cout << "EMPTYY!!\n";
            generateRandomPath(positionEnemy, path);
        }
    }else{
        generateRandomPath(positionEnemy, path);
    }
    //Generar camino aleatorio
}

bool pathFinding(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> & path)
{
    path.clear();
    Heap<ANode_Ptr> frontera;
    std::map<vec2i, ANode_Ptr> expanded;
    sf::Vector2i objetive = selectCloseObjetive(positionEnemy, objetives);

    ANode_Ptr currentNode = std::make_shared<ANode>(ANode(positionEnemy, sf::Vector2i(0, 0), objetive, 0.0f));

    bool finded = false;
    while (!finded)
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
                    if (checkValidPosition(nodePosition) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(currentNode))
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
            finded = true;
        }
    }

    std::list<ANode_Ptr> list_actions;
    //Si no se ha encontrado -> seleccionar aleatorio
    if (!finded)
    {
        int randomNode = Random::getIntNumberBetween(0, expanded.size() - 1);
        for (auto &p : expanded)
        {
            if (randomNode > 0)
            {
                randomNode--;
            }
            else
            {
                currentNode = p.second;
                break;
            }
        }
    }
    while (currentNode != nullptr)
    {
        if(currentNode->getParent() != nullptr){
            list_actions.push_back(currentNode);
        }
        currentNode = currentNode->getParent();
    }

    while (!list_actions.empty())
    {
        ANode_Ptr e = list_actions.back();
        path.push_back(e);
        list_actions.pop_back();
    }

    return finded;
}