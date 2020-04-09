#include "A_Star.hpp"

bool checkObjetive(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition)
{
    return currentP.x == objetivePosition.x && currentP.y == objetivePosition.y;
}

bool checkValidPosition(const sf::Vector2i &v)
{
    if(!EntityMap::isValidCell(v)){
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

std::list<ANode> &pathFinding(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives)
{
    Heap<ANode> frontera;
    std::map<vec2i, ANode *> expanded;
    sf::Vector2i objetive = selectCloseObjetive(positionEnemy, objetives);

    ANode *currentNode = new ANode(positionEnemy, sf::Vector2i(0, 0), objetive, 0.0f);

    bool finded = false;
    while (!finded)
    {

        expanded[vec2i(currentNode->getPosition())] = currentNode;
        //expandir nodos
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {   
                if(abs(i) != abs(j)){
                    sf::Vector2i nodePosition(currentNode->xPosition() + i, currentNode->yPosition() + j);
                    sf::Vector2i objetiveP = selectCloseObjetive(positionEnemy, objetives);
                    ANode *newNode = new ANode(nodePosition, sf::Vector2i(i, j), objetiveP, currentNode->fAcum() + 1, currentNode);
                    if (checkValidPosition(nodePosition) && expanded.count(vec2i(nodePosition)) == 0 && !frontera.containsNode(currentNode))
                    { //Si es una posicion valida y no se ha expandido
                        frontera.add(newNode);
                    }
                    else
                    {
                        delete newNode;
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

    std::list<ANode *> list_actions;
    std::list<ANode> *result = new std::list<ANode>();
    if (finded)
    {
        while (currentNode != nullptr)
        {
            list_actions.push_back(currentNode);
            ANode *aux = currentNode;
            currentNode = currentNode->getParent();
        }
        while (!list_actions.empty())
        {
            ANode * e = list_actions.back();
            result->push_back(*e);
            list_actions.pop_back();
        }
    }
    
    //limpiar nodos
   // for(std::map<vec2i, ANode*>::iterator it = expanded.begin(); it != expanded.end(); ++it){
   //     delete (it)->second;
   // }

    return *result;
}