#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Entities/Player.h"

int windowsHeight = 600;
int windowsWidth = 800;


int main(int argc, char* argv[]) {
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(windowsWidth, windowsHeight), "BomberMenaman");

	// Make maximun FPSs to 60. Somewhat control the player speed.
	window.setFramerateLimit(60);

	auto player = PlayerEntity();

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

		// TODO PLAYER MOVEMENT MUST NOT DEPENT ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
		player.update();

		// Clear screen from previous drawings
		window.clear();

		// Draw the player in the scene
		window.draw(player);

		// Update window
		window.display();
	}
}