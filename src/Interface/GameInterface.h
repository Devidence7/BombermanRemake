#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include "../Include/global.hpp"
#include <iostream>
#define MAX_NUMBER_OF_ITEMS 4
#define MAX_NUMBER_OF_PLAYERS 2
#define MAX_NUMBER_OF_PU 4
#define HEAD_DIM 36
#define HEAD_START 10

using namespace std;

class GameInterface {
    sf::RectangleShape container;
    sf::Font font;
    sf::Text lives[ MAX_NUMBER_OF_PLAYERS ];
    sf::Text time;
    int timeAMostrar;
    int timeLeft=299;
    int dimY = 15;
	int dimX = 25;

public:
	GameInterface(){
       this->container.setSize( sf::Vector2f((27) * sizeTextureX,  sizeTextureY));
        this->container.setFillColor(sf::Color(255,69,0,100));
        this->container.setPosition(0,-48);
       // this->container.setScale(1,0.5);
        if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }

        cout<<PLayers::getVectorPlayer().size()<<endl;
        for (int i=0;i< PLayers::getVectorPlayer().size();i++){
            /* lifeSprite[i].setTexture(lifeTexture,false);
             lifeSprite[i].setScale(0.1,0.1);
             lifeSprite[i].setPosition(container.getPosition().x+(50*(i+1)),(container.getPosition().y+7.5));*/
              PLayers::getVectorPlayer()[i]->playerHead2.setPosition(container.getPosition().x+(100*(i+1)),(container.getPosition().y+7.5));
            PLayers::getVectorPlayer()[i]->playerHead.setPosition(container.getPosition().x+(100*(i+1)),(container.getPosition().y+7.5));

             lives[i].setFont(font);
            // lives[i].setFillColor(sf::Color::White);
            lives[i].setColor(sf::Color::White);
            lives[i].setString("X"+to_string(PLayers::getVectorPlayer()[i]->getLives()));
              lives[i].setPosition(PLayers::getVectorPlayer()[i]->playerHead2.getPosition().x+35,PLayers::getVectorPlayer()[i]->playerHead2.getPosition().y+7.5);
             lives[i].setScale(0.5,0.5);

        
        }
       
        time.setFont(font);
      //  time.setFillColor(sf::Color::White);
      time.setColor(sf::Color::White);
        time.setString(to_string(timeLeft/60)+":"+to_string(timeLeft%60));
        time.setPosition(container.getPosition().x+(container.getSize().x/2)-100,(container.getPosition().y+7.5));
        time.setScale(0.5,0.5);
    


           

    }

    void draw(sf::RenderWindow& w) {
       // w.draw(lifeSprite[0]);
       w.draw(container);
        for (int i=0;i< PLayers::getVectorPlayer().size();i++){
            w.draw(PLayers::getVectorPlayer()[i]->playerHead2);
            w.draw(PLayers::getVectorPlayer()[i]->playerHead);
		
            w.draw(lives[i]);
        }
        //w.draw(player[0]);
      
        w.draw(time);
        
	}


   void drawMulti(sf::RenderWindow& w) {
        w.draw(container);
        for (int i=0;i<MAX_NUMBER_OF_PLAYERS;i++){
            w.draw(PLayers::getVectorPlayer()[i]->playerHead);
           // w.draw(lifeSprite[i]);
            w.draw(lives[i]);
         //   w.draw(player[i]);
        }
        w.draw(time);
	}

    void update(double timeNow) {
        int i=0;
        for(Player_ptr &player : PLayers::getVectorPlayer()){
            lives[i].setString("X"+to_string(player->getLives()));
            cout<<player->getLives()<<endl;
               PLayers::getVectorPlayer()[i]->playerHead2.setPosition(container.getPosition().x+(100*(i+1)),(container.getPosition().y+7.5));
              PLayers::getVectorPlayer()[i]->playerHead.setPosition(container.getPosition().x+(100*(i+1)),(container.getPosition().y+7.5));
              lives[i].setPosition(PLayers::getVectorPlayer()[i]->playerHead2.getPosition().x+35,(PLayers::getVectorPlayer()[i]->playerHead2.getPosition().y+7.5));
            //numOfPowerUps[i]=j;
             i++;
        }
     //   lives1.setString("Lives: "+to_string(pe.getLives()));   
       // lives2.setString("Lives: "+to_string(pe2.getLives()));
        timeAMostrar=timeLeft-timeNow; 
        if(timeLeft<0){
            timeLeft=0;
        }  
        time.setString("Time: "+to_string( timeAMostrar/60)+":"+to_string( timeAMostrar%60));
	}

  

    
};
