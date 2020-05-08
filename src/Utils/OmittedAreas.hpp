#pragma once
#include <SFML/Graphics.hpp>

class OmittedArea{
    sf::Vector2i posOmited;

    public:

    OmittedArea(){}
    OmittedArea(sf::Vector2i p) : posOmited(p){}

    bool operator==(const sf::Vector2i other)const {
        return this->posOmited.x == other.x && this->posOmited.y == other.y;
    }
    bool operator==(const OmittedArea &other) const{
        return *this == other.posOmited;
    }

};