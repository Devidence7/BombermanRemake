#include <iostream>
#include <exception>
#include "GameEngine.hpp"
//
//Game::Game()
//{
//    //Pantalla completa max resolution
//    unsigned int x = VideoMode::getDesktopMode().width;
//    unsigned int y = VideoMode::getDesktopMode().height;
//    m_Window.create(VideoMode(x, y), "Bomberman", Style::Fullscreen);
//
//    // Load the background into the texture
//    // Be sure to scale this image to your screen size
//    if(!m_BackgroundTexture.loadFromFile("../Assets/map_sprites.png")){
//        throw std::exception();   
//    }
// 
//    // Associate the sprite with the texture
//    m_BackgroundSprite.setTexture(m_BackgroundTexture);
// 
//}
//
//
//
//void Game::start()
//{
//    // Timing
//    Clock clock;
// 
//    while (m_Window.isOpen())
//    {
//        // Restart the clock and save the elapsed time into dt
//        Time dt = clock.restart();
// 
//        // Make a fraction from the delta time
//        float dtAsSeconds = dt.asSeconds();
// 
//        //input();
//        update(dtAsSeconds);
//        draw();
//    }
//}
//
//void Game::update(float dtAsSeconds)
//{
//    //objects update
//    //m_Bob.update(dtAsSeconds);
//}
//
//void Game::draw()
//{
//    // Rub out the last frame
//    m_Window.clear(Color::White);
// 
//    // Draw the background
//    m_Window.draw(m_BackgroundSprite);
//    //m_Window.draw(m_Bob.getSprite());
// 
//    // Show everything we have just drawn
//    m_Window.display();
//}