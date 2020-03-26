#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <memory>
#include "GameEngine.hpp"
#include "Logic/Time.h"
#include "Logic/Random.h"
#include "Interface/MainMenu.h"
//#include "Map/Map.hpp"

int windowsHeight = 600;
int windowsWidth = 800;
bool primero=true;


int main(int argc, char *argv[])
{
	//Inicializar Texturas

	//Inicializar ventana

	// Create the main window
	//unsigned int desktopWidth = sf::VideoMode::getDesktopMode().width;
	//unsigned int desktopHeight = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(27 * 48, 17 * 48), "Bombermenaman"); // sf::Style::Fullscreen
	
	// Make maximum FPSs to 60. Somewhat control the player speed.
	window.setFramerateLimit(60);

	// Start counting the time:
	GameTime();

	// Start Random Generator
	Random::initilizeRandomGen();
	

	Game game;
	MainMenu menu(window);

	sf::Music music;
	if (!music.openFromFile("../music/Title.flac"))
		return -1; // error
	music.play();

	// Start game loop
	while (window.isOpen())
	{		
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::KeyReleased:
					switch (event.key.code){
						case sf::Keyboard::Up:
							menu.moveUp();
							break;

						case sf::Keyboard::Down:
							menu.moveDown();
							break;
						
						case sf::Keyboard::P:
							if(menu.itemSelected()==0){
								if (!music.openFromFile("../music/World 1.flac"))
									return -1; // error
								music.play();
								primero=false;
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
		if(primero){
			menu.draw(window);
		}
		else{
		// TODO PLAYER MOVEMENT MUST NOT DEPEND ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
			game.update();

		// Clear screen from previous drawings
			window.clear();

		// Draw the player and the scene
			game.draw(window);
		}

		// Update window
		window.display();


	}
}