#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "GameEngine.hpp"
#include "Logic/Time.h"
#include "Logic/Random.h"
#include "Interface/MainMenu.h"
#include "Music/GameMusic.h"

//#include "Map/Map.hpp"

int windowsHeight = 600;
int windowsWidth = 800;
bool primero = true;
bool multi=false;
int numPlayers;
list<int> playersLives;


int main(int argc, char* argv[]) {
	//Inicializar Texturas

	//Inicializar ventana

	// Create the main window
	//unsigned int desktopWidth = sf::VideoMode::getDesktopMode().width;
	//unsigned int desktopHeight = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(27 * 48, 17 * 48), "Bombermenaman"); // sf::Style::Fullscreen

	// Make maximum FPSs to 60. Somewhat control the player speed.
	window.setFramerateLimit(60);

	// Start counting the time:
	GameTime time=GameTime();

	// Start Random Generator
	Random::initilizeRandomGen();

	Game game=Game();
	MainMenu menu;
	GameInterface gameI=GameInterface();
	GameMusic::playTitleMusic();
	GameMusic::setVolume(10);

	// Start game loop
	while (window.isOpen()) {
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyReleased:
				switch (event.key.code) {
				case sf::Keyboard::Up:
					menu.moveUp();
					break;

				case sf::Keyboard::Down:
					menu.moveDown();
					break;

				case sf::Keyboard::P:
					if (menu.itemSelected() == 0) {
						GameMusic::playWorld1Music();
						primero = false;
						numPlayers=1;
					}

					else if (menu.itemSelected() == 1) {
						GameMusic::playWorld1Music();
						primero = false;
						multi=true;
						numPlayers=2;
					}
					else if (menu.itemSelected() == 3) {
						window.close();
					}
				}
				break;

				// window closed
			case sf::Event::Closed:
				// Close window -> exit
				window.close();
				break;
			case sf::Event::LostFocus:
				//Pause
				break;
			case sf::Event::GainedFocus:
				//resume
				break;

				// we don't process other types of events
			default:
				break;
			}
		}

		if (primero) {
			//game.start(window);
			menu.draw(window);
		}
		else {
			// TODO PLAYER MOVEMENT MUST NOT DEPEND ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
			game.update();

			// Clear screen from previous drawings
			window.clear();
		      
	        
			
			// Draw the player and the scene
		game.draw(window);
		for(Player_ptr &player : players){
			playersLives.push_back(player->getLives());
			cout<<playersLives.front();
		}
		if(multi){
			gameI.update(time.getTimeNow(),playersLives.front(),playersLives.back());
		 	gameI.drawMulti(window);
		}
		else{
			gameI.update(time.getTimeNow(),playersLives.front());
			gameI.draw(window);
		}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) { GameMusic::volumeUp(); std::cout << GameMusic::getVolume() << std::endl; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) { GameMusic::volumeDown(); std::cout << GameMusic::getVolume() << std::endl;}

		// Update window
		window.display();


	}
}