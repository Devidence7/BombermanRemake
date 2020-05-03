#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include "../Include/global.hpp"
#include <iostream>
#define MAX_NUMBER_OF_ITEMS 4
#define MAX_NUMBER_OF_PLAYERS 2
#define MAX_NUMBER_OF_PU 4
using namespace std;

class GameInterface {
    sf::RectangleShape container;
    sf::Font font;
    sf::Text lives[ MAX_NUMBER_OF_PLAYERS ];
    //sf::Sprite* powerUps[ MAX_NUMBER_OF_PLAYERS ][MAX_NUMBER_OF_PU];
    sf::Sprite lifeSprite[ MAX_NUMBER_OF_PLAYERS ];
    sf::Texture lifeTexture; 
    int numLives[ MAX_NUMBER_OF_PLAYERS ];
    sf::Text time;

    /*sf::Text player1;
    sf::Text player2;*/
    //sf::Text powerUps;
    int timeAMostrar;
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
        this->container.setFillColor(sf::Color(255,69,0,100));
        this->container.setPosition(0,-48);
       // this->container.setScale(1,0.5);
        if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }

      

        lifeTexture.loadFromFile("../textures/mainMenu/heart.png");
        for (int i=0;i<MAX_NUMBER_OF_PLAYERS;i++){
             lifeSprite[i].setTexture(lifeTexture,false);
             lifeSprite[i].setScale(0.1,0.1);
             lifeSprite[i].setPosition(container.getPosition().x+(50*(i+1)),(container.getPosition().y+7.5));

             lives[i].setFont(font);
             lives[i].setColor(sf::Color::White);
             lives[i].setString("X"+to_string(numLives[0]));
             lives[i].setPosition(lifeSprite[i].getPosition().x+25,(lifeSprite[i].getPosition().y+7.5));
             lives[i].setScale(0.5,0.5);

        
            /* lives[1].setFont(font);
             lives[1].setColor(sf::Color::White);
             lives[1].setString("X"+to_string(numLives[1]));
             lives[1].setPosition(lifeSprite[1].getPosition().x+150,(container.getPosition().y+7.5));
             lives[1].setScale(0.5,0.5);*/
        }
        /* lifeSprite[1].setTexture(lifeTexture,false);
             lifeSprite[1].setScale(0.1,0.1);
            // lifeSprite[1].setPosition(lives[1].getPosition().x-30,lives[1].getPosition().y-7.5);
             lifeSprite[1].setPosition(container.getPosition().x+(container.getSize().x/2)+200,(container.getPosition().y+7.5));*/
        
        time.setFont(font);
        time.setColor(sf::Color::White);
        time.setString(to_string(timeLeft/60)+":"+to_string(timeLeft%60));
        time.setPosition(container.getPosition().x+(container.getSize().x/2)-100,(container.getPosition().y+7.5));
        time.setScale(0.5,0.5);
    

      /*  player[1].setFont(font);
        player[1].setColor(sf::Color::White);
        //player[1].setString("Player 2");
        player[1].setPosition(container.getPosition().x+(container.getSize().x/2)+200,(container.getPosition().y+7.5));
        player[1].setScale(0.5,0.5);*/

         /* player[0].setFont(font);
        player[0].setColor(sf::Color::White);
       // player[0].setString("Player 1");
        player[0].setPosition(container.getPosition().x+50,(container.getPosition().y+7.5));
        player[0].setScale(0.5,0.5);*/

      
    
      /* for(int i=0;i<MAX_NUMBER_OF_PLAYERS;i++){
            powerUps[i][0]->setScale(0.5,0.5);
            powerUps[i][0]->setPosition(lives[i].getPosition().x+100,lives[i].getPosition().y);
            numOfPowerUps[i]=0;
        }*/
        /*for(Player_ptr &player : PLayers::getVectorPlayer()){
           player->activatedPowerUps.front()->setScale(0.1,0.1);
            player->activatedPowerUps.front()->setPosition(lives[i].getPosition().x+30,lives[i].getPosition().y-7.5);
        }*/
        

         

    }

    void draw(sf::RenderWindow& w) {
        w.draw(lifeSprite[0]);
		w.draw(container);
        w.draw(lives[0]);
        //w.draw(player[0]);
       
       /* if(numOfPowerUps[0]>0){
            for(int i=0;i<=numOfPowerUps[0];i++){
                w.draw(powerUps[0][i]);
            }
        }*/
        int i=0;
       /*    for(Player_ptr &player : PLayers::getVectorPlayer()){
          if(player->activatedPowerUps.size()>0){
                 for(int j=0;j<player->activatedPowerUps.size();j++){
                      cout<<player->activatedPowerUps.size()<<endl;
                       w.draw(*powerUps[i][j]);
                 }
             }
             i++;
         }*/
        w.draw(time);
        
	}


   void drawMulti(sf::RenderWindow& w) {
        w.draw(container);
        for (int i=0;i<MAX_NUMBER_OF_PLAYERS;i++){
            w.draw(lifeSprite[i]);
            w.draw(lives[i]);
         //   w.draw(player[i]);
        }
        w.draw(time);
	}

    void update(double timeNow) {
        int i=0;
        for(Player_ptr &player : PLayers::getVectorPlayer()){
            lives[i].setString("X"+to_string(player->getLives()));
           
            int j=0;
            //EL iterador se pone al inicio de una lista que no existe si no hay power ups. Asegurse de que activatedPU no esta vacio.Contar powerUps activos
           /* for(list<sf::Sprite*>::iterator ci = player->activatedPowerUps.begin(); ci !=player->activatedPowerUps.end(); ++ci){
              //  cout<<"I:"<<i<<" J:"<<j<<endl;
                powerUps[i][j]=(*ci);
                if(j!=0){
                    powerUps[i][j]->setPosition(powerUps[i][j-1]->getPosition().x+25,powerUps[i][j-1]->getPosition().y);
                }
                j++;
            }*/
            
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

   /* void update(double timeNow,PlayerEntity &pe) {

        lives1.setString("Lives: "+to_string(pe.getLives()));   
        timeLeft=timeLeft-timeNow; 
        if(timeLeft<0){
            timeLeft=0;
        }  
        time.setString("Time: "+to_string(timeLeft/60)+":"+to_string(timeLeft%60));
	}*/

    
};
