#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Game{
private:
    // A regular RenderWindow
    RenderWindow m_Window;  
 
    // Declare a sprite and a Texture for the background
    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;
 
    // An instance of Bob
    //Bob m_Bob;
 
    // Private functions for internal use only
    //void input();
    void update(float dtAsSeconds);
    void draw();
 
public:
    // The Game constructor
    Game();
    // start will call all the private functions
    void start();
};

