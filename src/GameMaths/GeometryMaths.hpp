#pragma once

#include <math.h>

#include <SFML/Graphics.hpp>

#include "../Entities/Entity.h"
#include "../Include/EntitiesInclude.hpp"

//Basic operators

inline float calculate_penetration(float r1, float r2, float d)
{
    return (r1 + r2 - d);
}

inline float dot(sf::Vector2f &v1, sf::Vector2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

inline float moduleVector(sf::Vector2f &v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}
inline float moduleVector(const sf::Vector2f &v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

//Poligon maths

inline float distance_to_squere(sf::Vector2f origin, sf::Vector2f dir, sf::Vector2f N, sf::Vector2f pRect)
{
    float denom = dot(N, dir);
    sf::Vector2f p0 = pRect - origin;
    return dot(p0, N) / denom;
}

inline float intersectPoint_planeVector(sf::Vector2f origin, sf::Vector2f dir, sf::Vector2f N, sf::Vector2f pRect)
{
    float denom = dot(N, dir);
    sf::Vector2f p0 = pRect - origin;
    return dot(p0, N) / denom;
}
