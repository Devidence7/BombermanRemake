#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>
#include "GUI/GameGUI.hpp"
#include "GUI/Theme.hpp"
#include "../Music/GameMusic.h"

#include "InterfacePaths.h"
#include "GameInterface.h"
#include "../GameEngine.hpp"
#include "GameDisplayController.h"

using namespace std;



class LoadingScreen{
    private:
    sf::RectangleShape background;
    sf::Font font;
    sf::Text stageText;
    sf::Text loading;
public:
LoadingScreen(sf::RenderWindow& window,Game game){
	background.setFillColor(sf::Color(0, 0, 0));
     if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }

        float resizeVal = std::fmax((float)window.getSize().x /8 , (float)window.getSize().y/8);
		background.setScale(resizeVal, resizeVal);
        
      stageText.setFont(font);
      //stageText.setFillColor(sf::Color::White);
      stageText.setColor(sf::Color::White);
      stageText.setString("NIVEL " + to_string(game.stage));
      stageText.setPosition(sf::Vector2f(window.getSize().x / 2-(window.getSize().x/9) , window.getSize().y/2- (window.getSize().y/9)));
      stageText.setScale(2,2);

      loading.setFont(font);
      //loading.setFillColor(sf::Color::White);
      loading.setColor(sf::Color::White);
      loading.setString("Cargando...");
      loading.setPosition(sf::Vector2f(window.getSize().x /2-(window.getSize().x/12), (window.getSize().y/2+200)));
      loading.setScale(1,1);

 }


 void draw(sf::RenderWindow& window) {
     window.draw(background);
     window.draw(stageText);
     window.draw(loading);
 }
};
