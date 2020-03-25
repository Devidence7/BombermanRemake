#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include <iostream>
#define MAX_NUMBER_OF_ITEMS 3
using namespace std;

class MainMenu {
  //  RectangleShape container;
    sf::Font font;
    sf::Text menuOption[MAX_NUMBER_OF_ITEMS];
    int selectedItem;
    sf::Sprite background;
    sf::Texture texture; 
    int dimY = 15;
	int dimX = 25;


	

public:
	MainMenu(sf::RenderWindow& window){
        /*this->container.setSize(
            sf::Vector2f(
            static_cast<float>(window.getSize().x) /4.f,
            static_cast<float>(window.getSize().y) /2.f

            )
        );
        this->container.setFillColor(sf::Color(20,20,20,200));
        this->container.setPosition(
             static_cast<float>(window.getSize().x) /2.f,
              static_cast<float>(window.getSize().y) /2.f
        );*/
        if(!font.loadFromFile("../textures/mainMenu/OpenSans-Bold.ttf")){
            //cosas
        }
    
        menuOption[0].setFont(font);
        menuOption[0].setColor(sf::Color::Red);
        menuOption[0].setString("Empezar juego");
       menuOption[0].setPosition((window.getSize().x) /2.f,(window.getSize().y/(MAX_NUMBER_OF_ITEMS+1)));


       menuOption[1].setFont(font);
        menuOption[1].setColor(sf::Color::White);
        menuOption[1].setString("Opciones");
        menuOption[1].setPosition((window.getSize().x) /2.f,(window.getSize().y/(MAX_NUMBER_OF_ITEMS+1)*2));


        menuOption[2].setFont(font);
        menuOption[2].setColor(sf::Color::White);
        menuOption[2].setString("Salir");
        menuOption[2].setPosition((window.getSize().x) /2.f,(window.getSize().y/(MAX_NUMBER_OF_ITEMS+1)*3));

       // background.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
      // background.setSize(sf::Vector2f((dimX + 2) * sizeTextureX, (dimY + 2) * sizeTextureY));
       
       texture.loadFromFile(MAIN_MENU_BACKGROUND_PATH);
	    background.setTexture(texture,true);
        background.setScale(0.81,0.82);
        selectedItem=0;


    }


    void moveUp(){
        int anteriorItem=selectedItem;
        selectedItem--;
        if(selectedItem<0){
            selectedItem=MAX_NUMBER_OF_ITEMS-1;
        }
         menuOption[anteriorItem].setColor(sf::Color::White);
         menuOption[selectedItem].setColor(sf::Color::Red);

    }

    void moveDown(){
        int anteriorItem=selectedItem;
        selectedItem=(selectedItem+1)%MAX_NUMBER_OF_ITEMS;
        menuOption[selectedItem].setColor(sf::Color::Red);
        menuOption[anteriorItem].setColor(sf::Color::White);
    }
    
    void draw(sf::RenderWindow& w) {
	//	w.draw(container);
    //    w.draw(background);
        for (int i=0;i<MAX_NUMBER_OF_ITEMS;i++){
           
            w.draw(menuOption[i]);
        }
	}

    int itemSelected(){
        return selectedItem;
    }

    
};
