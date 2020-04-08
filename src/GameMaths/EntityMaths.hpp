#pragma once

#include <SFML/Graphics.hpp>

#include "../Include/EntitiesInclude.hpp"
#include "../GameMaths/GeometryMaths.hpp"

bool circIntersect(Entity &e1, Entity &e2)
{
    sf::FloatRect shape1 = e1.getGlobalBounds();
    sf::FloatRect shape2 = e2.getGlobalBounds();

    float dx = (shape1.left + shape1.width / 2) - (shape2.left + (shape2.width / 2));
    float dy = (shape1.top + (shape1.width / 2)) - (shape2.top + (shape2.width / 2));
    float distance = std::sqrt((dx * dx) + (dy * dy));

    return (distance <= (shape1.width / 2) + (shape2.width / 2));
}

bool intersectsCircleRect(Entity &e1circle, Entity &e2rect, double &x, double &y)
{
    sf::FloatRect circle = e1circle.getGlobalBounds();
    sf::Vector2f circleC = e1circle.getCenterPosition();
    sf::FloatRect rect = e2rect.getGlobalBounds();
    sf::Vector2f recteC = e2rect.getCenterPosition();

    sf::Vector2f circleDistance;

    //std::cout << "1" << std::endl;

    x = (circleC.x - recteC.x);
    y = (circleC.y - recteC.y);
    circleDistance.x = abs(x);
    circleDistance.y = abs(y);

    //std::cout << "2" << std::endl;

    // circle.width and circle.height should be the same (it is the radius).
    if (circleDistance.x > (rect.width / 2 + circle.width / 2))
    {
        return false;
    }
    if (circleDistance.y > (rect.height / 2 + circle.height / 2))
    {
        return false;
    }

    // Move one pixel modulus
    float moduloCentro = sqrtf(x * x + y * y);
    x /= moduloCentro;
    y /= moduloCentro;

    x /= 1000.0;
    y /= 1000.0;

    if (circleDistance.x <= (rect.width / 2))
    {
        return true;
    }
    if (circleDistance.y <= (rect.height / 2))
    {
        return true;
    }

    double cornerDistance_sq;
    cornerDistance_sq = pow((circleDistance.x - rect.width / 2), 2) +
                        pow((circleDistance.y - rect.height / 2), 2);

    return (cornerDistance_sq <= pow(circle.width / 2, 2));
}