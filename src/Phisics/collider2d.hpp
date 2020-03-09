#pragma once
#include <SFML/Graphics.hpp>


class Collider2d{
    protected:
    sf::FloatRect collider;
    sf::Vector2f position;
    //Este elemento se puede destruir
    bool destroyable = false;
    //Este elemento puede destruir otros elementos
    bool destroyer = false;

    public:
    Collider2d(){}
    Collider2d(sf::Vector2f _position, sf::FloatRect _collider, 
        bool _destroyer = false, bool _destroyale = false) 
        : position(_position), collider(_collider), destroyable(_destroyale), destroyer(_destroyer)
    {}

    inline sf::FloatRect getRect(){
        return this->collider;
    }

    inline bool isDestoyer() const {return this->destroyer;}
    inline bool isDestroyable() const {return this->destroyable;}
    
    bool isCollision(const Collider2d &col) const {
        return this->collider.intersects(col.collider);
    }
};