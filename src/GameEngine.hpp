#pragma once
#include <SFML/Graphics.hpp>
#include <list>

#include "Exceptions/ExceptionsGame.hpp"

using namespace sf;


/**
 * 
 * Esta clase se encarga de gestionar la lógica del juego
 * 
 */
 
class Level
{
    int level = 1;
    Sprite BackgroundSprite;
    Texture BackgroundTexture;
    std::list<Sprite> enemies;
    std::list<Sprite> walls;

public:
    Level() {}
    Level(int _level) : level(_level)
    {
        if (!this->BackgroundTexture.loadFromFile("../Assets/map_sprites.png"))
        {
            throw ExceptionLoadImage("Error al cargar imagen de fondo");
        }
        this->BackgroundSprite = Sprite(this->BackgroundTexture);
        //sf::IntRect frame(0, 0, 496, 225);
        //BackgroundSprite.setTextureRect(frame);
        //BackgroundSprite.setTexture(BackgroundTexture);
        
        //Instanciar pilares
        //Instanciar jugador
        //Instanciar enemigos (Guardar margen con jugador de X casillas)
        //Instanciar muros
        

        // Associate the sprite with the texture
    }

    Sprite & getBackground(){
        return this->BackgroundSprite;
    }

    void draw(RenderWindow *w);
};


/**
 * 
 * Clase encargada de inicialización de niveles.
 * Mostrará al comienzo el menú
 * Mostrará al morir el Game Over
 */ 

class Game
{
private:
    RenderWindow m_Window;
    Level level;
    Sprite BackgroundSprite;
    Texture BackgroundTexture;
    void update(float dtAsSeconds);
    void draw();

public:
    Game();
    void start();
};
