#include <iostream>
#include <exception>
#include "GameEngine.hpp"

void Level::draw(RenderWindow *w)
{
    w->draw(this->BackgroundSprite);
    //w->draw(this->BackgroundSprite);
    //for (Sprite &enemy : this->enemies)
    //{
    //    w->draw(enemy);
    //}
    //for (Sprite wall : this->walls)
    //{
    //    w->draw(wall);
    //}
}

Game::Game()
{
    //Pantalla completa max resolution
    unsigned int x = VideoMode::getDesktopMode().width;
    unsigned int y = VideoMode::getDesktopMode().height;
    m_Window.create(VideoMode(x, y), "Bomberman", Style::Fullscreen);
    if (!this->BackgroundTexture.loadFromFile("../Assets/map_sprites.png"))
    {
        throw ExceptionLoadImage("Error al cargar imagen de fondo");
    }
    this->BackgroundSprite = Sprite(this->BackgroundTexture);
    this->level = Level(1);

    // Load the background into the texture
    // Be sure to scale this image to your screen size
}

void Game::start()
{
    // Timing
    Clock clock;

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        // Make a fraction from the delta time
        float dtAsSeconds = dt.asSeconds();
        sf::sleep(sf::milliseconds(15));

        //input();
        //update(dtAsSeconds);
        draw();
    }
}

void Game::update(float dtAsSeconds)
{
    //objects update
    //m_Bob.update(dtAsSeconds);
}

void Game::draw()
{
    // Rub out the last frame
    m_Window.clear(Color::Black);
    //m_Window.draw(this->BackgroundSprite);
    this->level.draw(&m_Window);
    //m_Window.draw(m_Bob.getSprite());

    // Show everything we have just drawn
    m_Window.display();
}