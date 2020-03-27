#pragma once
#include <SFML/Graphics.hpp>
#define MAX_NUMBER_OF_ITEMS 3

class PauseMenu {
    sf::RectangleShape container;
    sf::Font font;
    sf::Text menuOption[MAX_NUMBER_OF_ITEMS];
    int selectedItem;

	

public:
	PauseMenu(sf::RenderWindow& window){
        this->container.setSize(
            sf::Vector2f(
            static_cast<float>(window.getSize().x) /4.f,
            static_cast<float>(window.getSize().y) /2.f

            )
        );
        this->container.setFillColor(sf::Color(20,20,20,200));
        this->container.setPosition(
             static_cast<float>(window.getSize().x) /2.f,
              static_cast<float>(window.getSize().y) /2.f
        );
        menuOption[0].setFont(font);
        menuOption[0].setColor(sf::Color::Red);
        menuOption[0].setString("Continuar");
        menuOption[0].setPosition(container.getPosition().x/2,container.getPosition().y/2);


        menuOption[1].setFont(font);
        menuOption[1].setColor(sf::Color::White);
        menuOption[1].setString("Opciones");
        menuOption[1].setPosition(container.getPosition().x,container.getPosition().y/2);


        menuOption[2].setFont(font);
        menuOption[2].setColor(sf::Color::White);
        menuOption[2].setString("Salir");
        menuOption[2].setPosition(container.getPosition().x,container.getPosition().y/3);

        selectedItem=0;


    }


    void moveUp(){
        int anteriorItem=selectedItem;
        selectedItem--;
        if(selectedItem<0){
            selectedItem=MAX_NUMBER_OF_ITEMS-1;
        }
         menuOption[selectedItem].setColor(sf::Color::White);
         menuOption[selectedItem].setColor(sf::Color::Red);

    }

    void moveDown(){
        int anteriorItem=selectedItem;
        (selectedItem++)%(MAX_NUMBER_OF_ITEMS-1);
        menuOption[selectedItem].setColor(sf::Color::Red);
        menuOption[selectedItem].setColor(sf::Color::White);
    }
    
    void draw(sf::RenderWindow& w) {
	//	w.draw(*container);
        for (int i=0;i<MAX_NUMBER_OF_ITEMS;i++){
            w.draw(menuOption[i]);
        }
	}

    void update(){
        
    }
};
