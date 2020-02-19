#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>

int tamImageX = 1200, tamImgaeY = 600;
float DireccionX = 0.4, DireccionY = 1;

int tamSpriteX = 0, tamSpriteY = 0;

int actualizarShape(float &x, float &y)
{
    x += DireccionX;
    y += DireccionY;
    if (x < 0 || x + tamSpriteX*2 > tamImageX)
    {
        //rebotar
        DireccionX = -DireccionX;
    }

    if (y < 0 || y + tamSpriteY*2 > tamImgaeY)
    {
        std::cout << y << std::endl;
        DireccionY = -DireccionY;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(tamImageX, tamImgaeY), "Rebotes!");
    sf::CircleShape shape(100.f);
    //sf::RectangleShape shape(100.f)
    shape.setFillColor(sf::Color::Green);
    //textura
    sf::Texture texture;
    if (!texture.loadFromFile("image.jpeg"))
    {
    }
    tamSpriteX = texture.getSize().x / 2;
    tamSpriteY = texture.getSize().y / 2;

    float posX = 0, posY = 0;

    //if (!texture.loadFromFile("image.jpeg", sf::IntRect(32, 32, 32, 32)))
    //{
    //    // error...
    //}
    sf::Sprite sprite;
    sprite.setPosition(sf::Vector2f(tamImageX / 2, tamImgaeY / 2));
    sprite.setTexture(texture);
    window.clear();
    //window.draw(shape);
    window.draw(sprite);
    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        //window.draw(shape);
        window.draw(sprite);
        window.display();
        sleep(0.3);
        actualizarShape(posX, posY);
        sprite.setPosition(sf::Vector2f(posX, posY));
    }

    return 0;
}