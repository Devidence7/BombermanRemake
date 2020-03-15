#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "GameEngine.hpp"
#include "Logic/Time.h"
//#include "Map/Map.hpp"

int windowsHeight = 600;
int windowsWidth = 800;


int main(int argc, char *argv[])
{
	//Inicializar Texturas

	//Inicializar ventana

	// Create the main window
	//unsigned int desktopWidth = sf::VideoMode::getDesktopMode().width;
	//unsigned int desktopHeight = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(27 * 48, 17 * 48), "BomberMenaman"); // sf::Style::Fullscreen
	
	// Make maximum FPSs to 60. Somewhat control the player speed.
	window.setFramerateLimit(60);

	// Start counting the time:
	GameTime::GameTime();
	

	Game game;
	// Start game loop
	while (window.isOpen())
	{		
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
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

		// TODO PLAYER MOVEMENT MUST NOT DEPEND ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
		game.update();

		// Clear screen from previous drawings
		window.clear();

		// Draw the player and the scene
		game.draw(window);

		// Update window
		window.display();
	}
}