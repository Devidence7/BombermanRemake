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
    int stage=1;
public:
LoadingScreen(sf::RenderWindow& window){
   
	background.setFillColor(sf::Color(0, 0, 0));
     if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }

        float resizeVal = std::fmax((float)window.getSize().x /8 , (float)window.getSize().y/8);
		background.setScale(resizeVal, resizeVal);
        
      stageText.setFont(font);
      stageText.setColor(sf::Color::White);
      stageText.setString("STAGE " + to_string(stage));
      stageText.setPosition(sf::Vector2f(window.getSize().x / 2-(window.getSize().x/9) , window.getSize().y/2- (window.getSize().y/9)));
      stageText.setScale(2,2);

      loading.setFont(font);
     loading.setColor(sf::Color::White);
      loading.setString("Loading...");
      loading.setPosition(sf::Vector2f(window.getSize().x /2-(window.getSize().x/12), (window.getSize().y/2+200)));
      loading.setScale(1,1);

 }
 void updateStage(int newStage){
     stage=newStage;
 }

 void draw(sf::RenderWindow& window) {
     window.draw(background);
     window.draw(stageText);
     window.draw(loading);
 }
};
