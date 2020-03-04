#include <SFML/Graphics.hpp>

#include "Entities/Player.h"
#include "GameEngine.hpp"
//#include "Map/Map.hpp"

int windowsHeight = 600;
int windowsWidth = 800;

int main(int argc, char* argv[]) {

	//Inicializar Texturas
	
	
	//Inicializar ventana
	
	// Create the main window
	//unsigned int desktopWidth = sf::VideoMode::getDesktopMode().width;
	//unsigned int desktopHeight = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(800, 600), "BomberMenaman"); // sf::Style::Fullscreen

	// Make maximum FPSs to 60. Somewhat control the player speed.
	window.setFramerateLimit(60);
	
	Game game;
	// Start game loop
	while (window.isOpen()) {
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			// Close window -> exit
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// TODO PLAYER MOVEMENT MUST NOT DEPEND ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
		game.update();

		// Clear screen from previous drawings
		window.clear();
		game.draw(window);
		// Draw the player in the scene
		//window.draw(player);

		// Update window
		window.display();
	}
}