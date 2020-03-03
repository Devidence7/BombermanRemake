#pragma once
#include <SFML/Graphics.hpp>
 
using namespace sf;
 
class Bat
{
private:
    Vector2f position;
 
    // A RectangleShape object
    RectangleShape batShape;
 
    float batSpeed = .3f;
 
public:
    Bat(float startX, float startY);
 
    FloatRect getPosition();
 
    RectangleShape getShape();
 
    void moveLeft();
 
    void moveRight();
 
    void update();
 
};

 
// This the constructor and it is called when we create an object
Bat::Bat(float startX, float startY)
{
    position.x = startX;
    position.y = startY;
 
    batShape.setSize(sf::Vector2f(50, 5));
    batShape.setPosition(position);
}
FloatRect Bat::getPosition()
{
    return batShape.getGlobalBounds();
}
 
RectangleShape Bat::getShape()
{
    return batShape;
}
 
void Bat::moveLeft()
{
    position.x -= batSpeed;
}
 
void Bat::moveRight()
{
    position.x += batSpeed;
}
 
void Bat::update()
{
    batShape.setPosition(position);
}