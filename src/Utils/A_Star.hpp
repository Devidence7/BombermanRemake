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
    std::shared_ptr<ANode> parent;
    float f, h;

public:
    int numOfWalls = 0;
    ANode();
    ANode(float fAcum, const sf::Vector2i cp, int _numOfWalss ,std::shared_ptr<ANode> p = nullptr);
    ANode(const sf::Vector2i cp, float fAcum, int numOFBombsOrFires, bool bombOrNot,std::shared_ptr<ANode> p = nullptr);
    ANode(const sf::Vector2i cp, sf::Vector2i dir, const sf::Vector2i objetive, float fAcum, std::shared_ptr<ANode> p = nullptr);
    void inverseDirection();
    float costNode() const;
    void incrementCost(float x);
    bool isObjetive() const;
    void setParent(std::shared_ptr<ANode> p);
    const sf::Vector2i &getPosition() const;
    sf::Vector2i getAction() const;
    std::shared_ptr<ANode> getParent() const;
    bool operator<(const ANode &other) const;
    bool operator<=(const ANode &other) const;
    bool operator>(const ANode &other) const;
    bool operator>=(const ANode &other) const;
    bool operator==(const sf::Vector2i &otherPosition) const;
    bool operator==(const ANode &other) const;
    float fAcum();
    int xPosition();
    int yPosition();

};


typedef std::shared_ptr<ANode> ANode_Ptr;



inline bool checkObjetive(const sf::Vector2i &currentP, const sf::Vector2i &objetivePosition);
#include "../Entities/Entity.h"
bool checkValidPosition(const sf::Vector2i &v, std::shared_ptr<Entity> e);
//bool checkValidPositionOrDestroyer(const sf::Vector2i &v, std::shared_ptr<Entity> e);
sf::Vector2i selectFarObjetive(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives);

ANode_Ptr generateRandomMovement(std::shared_ptr<Entity> e,  sf::Vector2i fromPosition);
void generateRandomPath(sf::Vector2i position, std::list<ANode_Ptr> & path, std::shared_ptr<Entity> e);
void generatePath(std::shared_ptr<Entity> e, std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> & path, TypeSeekIA typeSeek);
                             
//sf::Vector2i selectCloseObjetive(std::shared_ptr<Entity> e, const std::vector<sf::Vector2i> &objetives);
sf::Vector2i selectCloseObjetive(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives);
bool pathFinding(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> & path, std::shared_ptr<Entity> e,  TypeSeekIA typeSeek, bool alternativePath = true);
bool pathFindingBreakingWalls(const sf::Vector2i &positionEnemy, const std::vector<sf::Vector2i> &objetives, std::list<ANode_Ptr> &path, std::shared_ptr<Entity> e, TypeSeekIA typeSeek, int costAddDestroy);
