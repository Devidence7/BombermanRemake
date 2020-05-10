#pragma once
#include <SFML/Graphics.hpp>
#include "../Include/global.hpp"
#include "../GameMaths/GeometryMaths.hpp"
inline bool isOn(sf::Vector2i IA, sf::Vector2i o){
    return IA.x == o.x && IA.y == o.y;
}

inline bool isOn(sf::Vector2f IA, sf::Vector2f o){
    return (moduleVector(IA - o) < 3);
}


class PatrolState{
    public:
    sf::Vector2i objetivo1;
    sf::Vector2i objetivo2;

    PatrolState(){}
    PatrolState(sf::Vector2i o1, sf::Vector2i o2): objetivo1(o1), objetivo2(o2){}

    sf::Vector2i getObetive(sf::Vector2i IA){
        if(isOn(IA, objetivo1)){return objetivo2;}
        return objetivo1;
    }

};





