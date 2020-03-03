#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Entities/Player.h"

int windowsHeight = 600;
int windowsWidth = 800;


int main(int argc, char* argv[]) {
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(windowsWidth, windowsHeight), "Bomberman");

	// Make maximun FPSs to 60. Somewhat control the player speed.
	window.setFramerateLimit(60);

	// Texture
	sf::Texture bombermanTexture;
	if (!bombermanTexture.loadFromFile("../textures/Entities/WhiteBombermanSprites.png")) {
		std::cerr << "Unable to load texture:" << std::endl;
		std::cerr << "Texture location: ../textures/Entities/WhiteBombermanSprites.png" << std::endl;
		std::cerr << "Exec path: " << argv[0] << std::endl;
	}

	bool playerUp, playerDown, playerLeft, playerRight = false;
	auto player = PlayerEntity(bombermanTexture);

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

		// Player movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { playerRight = true; }
		else { playerRight = false; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { playerLeft = true; }
		else { playerLeft = false; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { playerUp = true; }
		else { playerUp = false; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { playerDown = true; }
		else { playerDown = false; }

		// TODO PLAYER MOVEMENT MUST NOT DEPENT ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
		player.update(playerDown, playerUp, playerLeft, playerRight);

		// Clear screen from previous drawings
		window.clear();

		// Draw the player in the scene
		window.draw(player);

		// Update window
		window.display();
	}
}