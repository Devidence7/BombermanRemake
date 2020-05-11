#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

class OmittedArea{
    sf::Vector2i posOmited;
    float tiempoHastaAparecer = 0;
    public:

    OmittedArea(){}
    OmittedArea(sf::Vector2i p, float _tiempo = 0) : posOmited(p), tiempoHastaAparecer(_tiempo){}

    bool operator==(const sf::Vector2i other)const {
        return this->posOmited.x == other.x && this->posOmited.y == other.y;
    }
    bool operator==(const OmittedArea &other) const{
        return *this == other.posOmited;
    }

    float TimeAp(){return tiempoHastaAparecer;}

};


class IntersetArea{
    float puntuacion = 0;
    sf::Vector2i posicion;

    public:
    IntersetArea(){}
    IntersetArea(sf::Vector2i pos, float interses) : puntuacion(interses), posicion(pos){}
    bool operator==(const sf::Vector2i other)const {
        return this->posicion.x == other.x && this->posicion.y == other.y;
    }
    bool operator==(const IntersetArea &other) const{
        return *this == other.posicion;
    }
    float intersest(){return puntuacion;}
};

typedef std::shared_ptr<IntersetArea> Interst_ptr;