#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
enum directionEnum{STOPED = 0, LEFT = 1, UP = 2, RIGHT = 2, DOWN = 4};

class Player{
    int lifes = 0;
    float speed = 0.0f;
    Sprite m_Sprite;
    Texture m_Texture;
    directionEnum direccion = STOPED;
 
    public:
        Player();
        inline void setSpeed(float _speed){this->speed = _speed;}
        inline void setLife(int _l){this->lifes = _l;}
        void updatePosition();
};