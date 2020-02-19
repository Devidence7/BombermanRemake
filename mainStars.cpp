#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <vector>
#include <tuple>
#include <random>

using namespace std;
int tamImageX = 1200, tamImgaeY = 600;
float DireccionX = 0.4, DireccionY = 1;

int tamSpriteX = 0, tamSpriteY = 0;
int MAXC = 10;
int countador = 0;

std::default_random_engine generator;
vector<tuple<float, float, sf::Sprite>> spriList;
sf::Texture texture;

void generateNewSprites()
{
    std::uniform_real_distribution<double> distribution(-1.0, 1);
    countador = countador % MAXC;
    if (!countador)
    {
        float x, y, norm;
        do
        {
            x = distribution(generator);
            y = distribution(generator);
            norm =x * x + y * y; 
        } while ((norm > 0.7) || norm < 0.3 );

        float posX = tamImageX / 2, posY = tamImgaeY / 2;

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(posX, posY));
        sprite.setScale(sf::Vector2f(0.01,0.01));
        spriList.push_back(tuple<float, float, sf::Sprite>(x, y, sprite));
    }
    countador++;
}

int actualizarShape(tuple<float, float, sf::Sprite> &t)
{
    float x = std::get<2>(t).getPosition().x, y = std::get<2>(t).getPosition().y;
    x += std::get<0>(t);
    y += std::get<1>(t);
    if (x+tamSpriteX < 0 || x  - tamSpriteX > tamImageX || y+tamSpriteX < 0 || y  - tamSpriteX> tamImgaeY)
    {
        return 0;
    }
    //  cout << std::get<2>(t).getPosition().x << " " << std::get<2>(t).getPosition().y << endl;
    std::get<2>(t).setPosition(sf::Vector2f(x, y));
    float sX = std::get<2>(t).getScale().x;
    float sY = std::get<2>(t).getScale().y;
    std::get<2>(t).setScale(sf::Vector2f( sX + 0.001, sY + 0.001));
    float rX = std::get<2>(t).getRotation();
    std::get<2>(t).setRotation(rX + 0.1);

    
    return 1;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(tamImageX, tamImgaeY), "Rebotes!");
    sf::CircleShape shape(100.f);
    //sf::RectangleShape shape(100.f)
    shape.setFillColor(sf::Color::Green);
    //textura
    //sf::Texture texture;
    if (!texture.loadFromFile("image.jpeg"))
    {
    }
    tamSpriteX = texture.getSize().x;
    tamSpriteY = texture.getSize().y;

    float posX = tamImageX / 2, posY = tamImgaeY / 2;

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(posX, posY));
    spriList.push_back(tuple<float, float, sf::Sprite>(0.3, -1, sprite));

    //window.draw(shape);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        generateNewSprites();
        //if(spriList.size() > 3){
        //    break;
        //}
        for (int i = 0; i < spriList.size();)
        {
            if (actualizarShape(spriList[i]))
            {
                window.draw(std::get<2>(spriList[i]));
                i++;
            }
            else
            {
                spriList.erase(spriList.begin() + i);
            }
        }
        window.display();
        sleep(0.6);
    }

    return 0;
}