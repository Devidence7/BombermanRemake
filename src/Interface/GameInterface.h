#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include "../Include/global.hpp"
#include <iostream>
#define MAX_NUMBER_OF_ITEMS 4
using namespace std;

class GameInterface {
    sf::RectangleShape container;
    sf::Font font;
    sf::Text lives1;
    sf::Text lives2;
    sf::Sprite lifeSprite;
    sf::Sprite lifeSprite2;
    sf::Texture lifeTexture; 
    int numLives=3;
    int numLives2=3;
    sf::Text time;
    sf::Text player1;
    sf::Text player2;
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
        this->container.setPosition(0,0);
       // this->container.setScale(1,0.5);
        if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }
    



        player1.setFont(font);
        player1.setColor(sf::Color::White);
        player1.setString("Player 1");
        player1.setPosition(container.getPosition().x+50,(container.getPosition().y+7.5));
        player1.setScale(0.5,0.5);




        lives1.setFont(font);
        lives1.setColor(sf::Color::White);
        lives1.setString("Lives: "+to_string(numLives));
        lives1.setPosition(player1.getPosition().x+150,(container.getPosition().y+7.5));
        lives1.setScale(0.5,0.5);

        


        player2.setFont(font);
        player2.setColor(sf::Color::White);
        player2.setString("Player 2");
        player2.setPosition(container.getPosition().x+(container.getSize().x/2)+200,(container.getPosition().y+7.5));
        player2.setScale(0.5,0.5);


        lives2.setFont(font);
        lives2.setColor(sf::Color::White);
        lives2.setString("Lives: "+to_string(numLives2));
        lives2.setPosition(player2.getPosition().x+150,(container.getPosition().y+7.5));
        lives2.setScale(0.5,0.5);

        


        time.setFont(font);
        time.setColor(sf::Color::White);
        time.setString("Time: "+to_string(timeLeft/60)+":"+to_string(timeLeft%60));
        time.setPosition(container.getPosition().x+(container.getSize().x/2)-100,(container.getPosition().y+7.5));
        time.setScale(0.5,0.5);
    

        lifeTexture.loadFromFile("../textures/mainMenu/heart.png");
        lifeSprite.setTexture(lifeTexture,false);
        lifeSprite.setScale(0.1,0.1);
        lifeSprite.setPosition(lives1.getPosition().x-30,lives1.getPosition().y-7.5);

        lifeSprite2.setTexture(lifeTexture,false);
        lifeSprite2.setScale(0.1,0.1);
        lifeSprite2.setPosition(lives2.getPosition().x-30,lives2.getPosition().y-7.5);
    


    }

    void draw(sf::RenderWindow& w) {
        w.draw(lifeSprite);
		w.draw(container);
        w.draw(lives1);
        w.draw(player1);
        w.draw(time);
        
	}


   void drawMulti(sf::RenderWindow& w) {
        w.draw(lifeSprite);
		w.draw(container);
        w.draw(lives1);
        w.draw(player1);
        w.draw(lifeSprite2);
        w.draw(lives2);
        w.draw(player2);
        w.draw(time);
        
	}

    void update(double timeNow,double lives1,double lives2) {

        lives1.setString("Lives: "+to_string(lives1));
        

         
        lives2.setString("Lives: "+to_string(lives2));
      

       timeLeft=timeLeft-timeNow; 
        if(timeLeft<0){
            timeLeft=0;
        }
       
        time.setString("Time: "+to_string(timeLeft/60)+":"+to_string(timeLeft%60));
        
        
	}

    
};
