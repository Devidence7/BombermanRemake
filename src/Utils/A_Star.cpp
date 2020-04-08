#include "A_Star.hpp"

bool checkObjetive(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition)
{
    return currentP.x == objetivePosition.x && currentP.y == objetivePosition.y;
}

bool checkValidPosition(const sf::Vector2i &v)
{
    return (v.x > 0 && v.y > 0 && v.x < EntityMap.size() && v.y < EntityMap[0].size()) && EntityMap[v.x][v.y] == nullptr;
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

std::vector<sf::Vector2i> &pathFinding(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives)
{
    Heap<ANode> frontera;
    std::map<vec2i, ANode *> expanded;
    sf::Vector2i objetive = selectCloseObjetive(positionEnemy, objetives);

    ANode *currentNode = new ANode(positionEnemy, sf::Vector2i(0, 0), objetive, 0.0f);

    expanded[vec2i(positionEnemy)] = nullptr;
    bool finded = false;
    while (!finded)
    {
        //expandir nodos
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                sf::Vector2i nodePosition(currentNode->xPosition() + i, currentNode->yPosition() + j);
                sf::Vector2i objetiveP = selectCloseObjetive(positionEnemy, objetives);
                ANode *newNode = new ANode(nodePosition, sf::Vector2i(i, j), objetiveP, currentNode->fAcum(), currentNode);
                if (checkValidPosition(nodePosition) && !expanded.count(vec2i(nodePosition)) && !frontera.containsNode(currentNode))
                { //Si es una posicion valida y no se ha expandido
                    frontera.add(newNode);
                }
                else
                {
                    delete newNode;
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

    std::list<sf::Vector2i> list_actions;
    std::vector<sf::Vector2i> *result = new std::vector<sf::Vector2i>();
    if (finded)
    {
        while (currentNode != nullptr)
        {
            list_actions.push_back(currentNode->getAction());
            ANode *aux = currentNode;
            currentNode = currentNode->getParent();
        }
        while (!list_actions.empty())
        {
            sf::Vector2i i = list_actions.back();
            sf::Vector2i j = i;
            list_actions.pop_back();
            result->push_back(j);
        }
    }
    return *result;
}