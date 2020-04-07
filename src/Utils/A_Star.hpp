#pragma once

#include <math.h>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <SFML/Graphics.hpp>

#include "heap.hpp"

#include "../Include/EntitiesInclude.hpp"


typedef std::shared_ptr<Entity> Entity_ptr;
typedef std::shared_ptr<PlayerEntity> Player_ptr;

inline float manhattan (const sf::Vector2i & currentP, const sf::Vector2i & objetivePosition){
    sf::Vector2i dir = currentP - objetivePosition;
    return sqrt(dir.x*dir.x + dir.y*dir.y);
}

class vec2i{
    public:
    int x, y;
    vec2i(const sf::Vector2i &v) : x(v.x), y(v.y){}
    inline bool operator==(const vec2i & other) const {
        return x == other.x && y == other.y;
    }
    inline bool operator<(const vec2i & other) const {
        if( x == other.x ){return y < other.y;}
        return x < other.x;
    }
    inline bool operator<=(const vec2i & other) const {
        if( x == other.x ){return y <= other.y;}
        return x < other.x;
    }
    inline bool operator>(const vec2i & other) const {
        if( x == other.x ){return y > other.y;}
        return x > other.x;
    }
    inline bool operator>=(const vec2i & other) const {
        if( x == other.x ){return y >= other.y;}
        return x > other.x;
    }
};


class ANode{
    sf::Vector2i currentPostion;
    sf::Vector2i direction_to_arrive;
    ANode * parent;
    float f, h;

public:
    ANode(const sf::Vector2i &cp, sf::Vector2i dir, const sf::Vector2i &objetive, float fAcum, ANode * p = nullptr) 
        : currentPostion(cp), direction_to_arrive(dir), f (fAcum), parent(p)
    {
        h = manhattan(cp, objetive);
    }
    inline float costNode() const {return h + f;}
    inline bool isObjetive() const { return h == 0;}
    inline void setParent(ANode *p){parent = p;}
    const sf::Vector2i & getPosition() const {return currentPostion;}
    sf::Vector2i getAction()const{return direction_to_arrive;}
    ANode * getParent()const{return parent;}
    inline bool operator<(const ANode &other) const{
        return (f+h) < other.costNode();
    }
    inline bool operator<=(const ANode &other) const{
        return (f+h) <= other.costNode();
    }
    inline bool operator>(const ANode &other) const{
        return (f+h) > other.costNode();
    }
    inline bool operator>=(const ANode &other) const{
        return (f+h) >= other.costNode();
    }
    
    inline bool operator==(const sf::Vector2i &otherPosition) const{
        return currentPostion.x == otherPosition.x && currentPostion.y == otherPosition.y;
    }

    inline bool operator==(const ANode &other) const{
        return (f+h) == other.costNode() && *this == other.currentPostion;
    }

    inline float fAcum(){return f;}
    inline int xPosition(){return currentPostion.x;}
    inline int yPosition(){return currentPostion.y;}

    ~ANode(){}
};



//class AStar{
namespace astar{


std::vector<std::vector<Entity_ptr>> map;
float sizeX;
float sizeY;

//public:
	
    //AStar(std::vector<std::vector<Entity_ptr>> &_m ) : map(_m){
    //    sizeX = map[0].size();
    //    sizeY = map.size();
    //}

    inline bool checkObjetive(const sf::Vector2i &currentP, const  sf::Vector2i & objetivePosition ){
        return currentP.x == objetivePosition.x && currentP.y == objetivePosition.y;
    }

    inline bool checkValidPosition(const sf::Vector2i &v) {
        return (v.x > 0 && v.y > 0 && v.x < map.size() && v.y < map[0].size()) && map[v.x][v.y] == nullptr;
    }


    std::vector<sf::Vector2i> & pathFinding(const sf::Vector2i & positionEnemy, const sf::Vector2i & positionObjetive){
	    Heap<ANode>frontera;
        std::map<vec2i, ANode *> expanded;
        ANode *currentNode = new ANode(positionEnemy, sf::Vector2i(0,0), positionObjetive, 0.0f);
        
        expanded[vec2i(positionEnemy)] = nullptr;
        bool finded = false;
        while (!finded)
        {
            //expandir nodos
            for(int i = -1; i < 2; i ++){
                for(int j = -1; j < 2; j++){
                    sf::Vector2i nodePosition(currentNode->xPosition() + i,currentNode->yPosition() + j);
                    ANode * newNode = new ANode(nodePosition, sf::Vector2i(i,j), positionObjetive, currentNode->fAcum(), currentNode);
                    if(checkValidPosition(nodePosition) && !expanded.count(vec2i(nodePosition)) && !frontera.containsNode(currentNode)){//Si es una posicion valida y no se ha expandido
                        frontera.add(newNode);
                    }else{
                        delete newNode;
                    }
                }
            }
            //Extraer nodo
            if(frontera.isEmpty()){
                break;
            }
            currentNode = frontera.pop();
            
            //currentNode = *
            if(currentNode->isObjetive()){
                finded = true;
            }
        }
        
        std::list<sf::Vector2i> list_actions;
        std::vector<sf::Vector2i> result;
        if(finded){
            while (currentNode != nullptr)
            {
                list_actions.push_back(currentNode->getAction());
                ANode * aux = currentNode;
                currentNode = currentNode->getParent();
            }
            while (!list_actions.empty())
            {
                sf::Vector2i i = list_actions.back() ;
                sf::Vector2i j = i;
                list_actions.pop_back();
                result.push_back(j);
            }
        }
        return result;

    }
}