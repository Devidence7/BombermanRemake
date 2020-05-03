#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include "../Include/global.hpp"
#include <iostream>
#define MAX_NUMBER_OF_ITEMS 4
// using namespace std;

class GameInterface {
    sf::RectangleShape container;
    sf::Font font;
    sf::Text lives[MAX_NUMBER_OF_ITEMS];
   /* sf::Text lives1;
    sf::Text lives2;*/
    sf::Sprite powerUps[MAX_NUMBER_OF_ITEMS];
    /*sf::Sprite powerUps;
    sf::Sprite powerUps2;*/
    sf::Texture powerUpTexture;
    sf::Sprite lifeSprite[MAX_NUMBER_OF_ITEMS];
   /* sf::Sprite lifeSprite;
    sf::Sprite lifeSprite2;*/
    sf::Texture lifeTexture; 
    int numLives[MAX_NUMBER_OF_ITEMS];
    /*int numLives=3;
    int numLives2=3;*/
    sf::Text time;
    sf::Text player[MAX_NUMBER_OF_ITEMS];

    /*sf::Text player1;
    sf::Text player2;*/
    //sf::Text powerUps;
    int timeLeft=299;
    int dimY = 15;
	int dimX = 25;

public:
	GameInterface(){
      /* this->container.setSize(
            sf::Vector2f(
            static_cast<float>(window.getSize().x),
            static_cast<float>(window.getSize().y) /20.f

            )
        );*/
       this->container.setSize( sf::Vector2f((27) * sizeTextureX,  sizeTextureY));
        this->container.setFillColor(sf::Color(0,0,0,100));
        this->container.setPosition(0,-48);
       // this->container.setScale(1,0.5);
        if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }

        player[0].setFont(font);
        player[0].setFillColor(sf::Color::White);
        player[0].setString("Player 1");
        player[0].setPosition(container.getPosition().x+50,(container.getPosition().y+7.5));
        player[0].setScale(0.5,0.5);




        lives[0].setFont(font);
        lives[0].setFillColor(sf::Color::White);
        lives[0].setString("Lives: "+to_string(numLives[0]));
        lives[0].setPosition(player[0].getPosition().x+150,(container.getPosition().y+7.5));
        lives[0].setScale(0.5,0.5);

        


        player[1].setFont(font);
        player[1].setFillColor(sf::Color::White);
        player[1].setString("Player 2");
        player[1].setPosition(container.getPosition().x+(container.getSize().x/2)+200,(container.getPosition().y+7.5));
        player[1].setScale(0.5,0.5);


        lives[1].setFont(font);
        lives[1].setFillColor(sf::Color::White);
        lives[1].setString("Lives: "+to_string(numLives[1]));
        lives[1].setPosition(player[1].getPosition().x+150,(container.getPosition().y+7.5));
        lives[1].setScale(0.5,0.5);

        


        time.setFont(font);
        time.setFillColor(sf::Color::White);
        time.setString("Time: "+to_string(timeLeft/60)+":"+to_string(timeLeft%60));
        time.setPosition(container.getPosition().x+(container.getSize().x/2)-100,(container.getPosition().y+7.5));
        time.setScale(0.5,0.5);
    

        lifeTexture.loadFromFile("../textures/mainMenu/heart.png");
        lifeSprite[0].setTexture(lifeTexture,false);
        lifeSprite[0].setScale(0.1,0.1);
        lifeSprite[0].setPosition(lives[0].getPosition().x-30,lives[0].getPosition().y-7.5);

        lifeSprite[1].setTexture(lifeTexture,false);
        lifeSprite[1].setScale(0.1,0.1);
        lifeSprite[1].setPosition(lives[1].getPosition().x-30,lives[1].getPosition().y-7.5);
    
    /*    powerUps.setScale(0.1,0.1);
        powerUps.setPosition(lives1.getPosition().x+30,lives1.getPosition().y-7.5);*/

         

    }

    void draw(sf::RenderWindow& w) {
        w.draw(lifeSprite[0]);
		w.draw(container);
        w.draw(lives[0]);
        w.draw(player[0]);
        w.draw(time);
        
	}


   void drawMulti(sf::RenderWindow& w) {
        w.draw(lifeSprite[0]);
		w.draw(container);
        w.draw(lives[0]);
        w.draw(player[0]);
        w.draw(lifeSprite[1]);
        w.draw(lives[1]);
        w.draw(player[1]);
        w.draw(time);
        
	}

    void update(double timeNow) {
        int i=0;
        for(Player_ptr &player : PLayers::getVectorPlayer()){
            lives[i].setString("Lives: "+to_string(player->getLives()));
            i++;
        }
     //   lives1.setString("Lives: "+to_string(pe.getLives()));   
       // lives2.setString("Lives: "+to_string(pe2.getLives()));
        timeLeft=timeLeft-timeNow; 
        if(timeLeft<0){
            timeLeft=0;
        }  
        time.setString("Time: "+to_string(timeLeft/60)+":"+to_string(timeLeft%60));
	}

   /* void update(double timeNow,PlayerEntity &pe) {

        lives1.setString("Lives: "+to_string(pe.getLives()));   
        timeLeft=timeLeft-timeNow; 
        if(timeLeft<0){
            timeLeft=0;
        }  
        time.setString("Time: "+to_string(timeLeft/60)+":"+to_string(timeLeft%60));
	}*/

    
};
