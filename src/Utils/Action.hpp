#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "IAglobal.hpp"
#include "A_Star.hpp"
class Action{
    public:
    TypeAction action;
    //ANode_Ptr currentNode;
    Action(TypeAction act): action(act) {    }

    bool operator==(Action o){
        return action == o.action;
    }

};
typedef std::shared_ptr<Action> Action_ptr;




class NodeAction {
    Action_ptr action;
    sf::Vector2i positionAction;
    std::shared_ptr<NodeAction> parent;
    float f, h;

public:

    NodeAction(const sf::Vector2i cp, const sf::Vector2i objetive, float fAcum, std::shared_ptr<NodeAction> p = nullptr)
        : positionAction(cp), f(fAcum), parent(p)
    {
        h = manhattan(cp, objetive);
       
    }

    inline float costNode() const { return h + f; }

    inline void incrementCost(float x){ f += x;  }

    inline bool isObjetive() const { return h == 0; }
    inline void setParent(std::shared_ptr<NodeAction> p) { parent = p; }
    const sf::Vector2i &getPosition() const { return positionAction; }
    std::shared_ptr<NodeAction> getParent() const { return parent; }
    inline bool operator<(const NodeAction &other) const
    {
        return (f + h) < other.costNode();
    }
    inline bool operator<=(const NodeAction &other) const
    {
        return (f + h) <= other.costNode();
    }
    inline bool operator>(const NodeAction &other) const
    {
        return (f + h) > other.costNode();
    }
    inline bool operator>=(const NodeAction &other) const
    {
        return (f + h) >= other.costNode();
    }

    inline bool operator==(const sf::Vector2i &otherPosition) const
    {
        return positionAction.x == otherPosition.x && positionAction.y == otherPosition.y;
    }

    inline bool operator==(const NodeAction &other) const
    {
        return (f + h) == other.costNode() && *this == other.positionAction;
    }

    inline float fAcum() { return f; }
    inline int xPosition() { return positionAction.x; }
    inline int yPosition() { return positionAction.y; }

    ~NodeAction() { action = nullptr; }
};

typedef std::shared_ptr<NodeAction> NodeAction_ptr;