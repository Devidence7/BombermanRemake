#pragma once

#include <math.h>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <SFML/Graphics.hpp>

#include "heap.hpp"


inline float manhattan(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition)
{
    sf::Vector2i dir = currentP - objetivePosition;
    return sqrt(dir.x * dir.x + dir.y * dir.y);
}

class vec2i
{
public:
    int x, y;
    vec2i(const sf::Vector2i &v) : x(v.x), y(v.y) {}
    inline bool operator==(const vec2i &other) const
    {
        return x == other.x && y == other.y;
    }
    inline bool operator<(const vec2i &other) const
    {
        if (x == other.x)
        {
            return y < other.y;
        }
        return x < other.x;
    }
    inline bool operator<=(const vec2i &other) const
    {
        if (x == other.x)
        {
            return y <= other.y;
        }
        return x < other.x;
    }
    inline bool operator>(const vec2i &other) const
    {
        if (x == other.x)
        {
            return y > other.y;
        }
        return x > other.x;
    }
    inline bool operator>=(const vec2i &other) const
    {
        if (x == other.x)
        {
            return y >= other.y;
        }
        return x > other.x;
    }
};

class ANode
{
    sf::Vector2i currentPostion;
    sf::Vector2i direction_to_arrive;
    ANode *parent;
    float f, h;

public:
    ANode(){
        direction_to_arrive = sf::Vector2i(0,0);
    }
    ANode(const sf::Vector2i &cp, sf::Vector2i dir, const sf::Vector2i &objetive, float fAcum, ANode *p = nullptr)
        : currentPostion(cp), direction_to_arrive(dir), f(fAcum), parent(p)
    {
        h = manhattan(cp, objetive);
        if(direction_to_arrive.x > 1 || direction_to_arrive.y >1 ){
		    std::cout<< "Error al construir accion\n";
		    volatile int x = 0;
	    }
    }

    inline void inverseDirection(){
        direction_to_arrive = -direction_to_arrive;
        currentPostion += direction_to_arrive;
        currentPostion += direction_to_arrive;
    }
    inline float costNode() const { return h + f; }
    inline bool isObjetive() const { return h == 0; }
    inline void setParent(ANode *p) { parent = p; }
    const sf::Vector2i &getPosition() const { return currentPostion; }
    sf::Vector2i getAction() const { return direction_to_arrive; }
    ANode *getParent() const { return parent; }
    inline bool operator<(const ANode &other) const
    {
        return (f + h) < other.costNode();
    }
    inline bool operator<=(const ANode &other) const
    {
        return (f + h) <= other.costNode();
    }
    inline bool operator>(const ANode &other) const
    {
        return (f + h) > other.costNode();
    }
    inline bool operator>=(const ANode &other) const
    {
        return (f + h) >= other.costNode();
    }

    inline bool operator==(const sf::Vector2i &otherPosition) const
    {
        return currentPostion.x == otherPosition.x && currentPostion.y == otherPosition.y;
    }

    inline bool operator==(const ANode &other) const
    {
        return (f + h) == other.costNode() && *this == other.currentPostion;
    }

    inline float fAcum() { return f; }
    inline int xPosition() { return currentPostion.x; }
    inline int yPosition() { return currentPostion.y; }

    ~ANode() {}
};


//class AStar{
//
//float sizeX;
//float sizeY;
//public:
//    AStar(std::vector<std::vector<Entity_ptr>> &_map) : sizeX(_map.size()), sizeY(_map[0].size()){}

inline bool checkObjetive(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition);

inline bool checkValidPosition(const sf::Vector2i &v);

ANode generateRandomMovement(sf::Vector2i fromPosition);
void generateRandomPath(sf::Vector2i position, std::list<ANode> & path);
void generatePath(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, int RangeVision, std::list<ANode> & path);

sf::Vector2i selectCloseObjetive(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives);

bool pathFinding(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode> & path);
#include "../Include/EntitiesInclude.hpp"
//};
