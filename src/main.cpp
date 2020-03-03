#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int windowsHeight = 600;
int windowsWidth = 800;

class PlayerEntity : public sf::Sprite {
public:
	sf::Vector2f velocity; // Speed of the Entity.
	float speedBoost;
	sf::Vector2f size; // Size of the Entity
	sf::Vector2f frameSize; // Size of the Entity Sprite

	std::vector<sf::IntRect> frames; // Frames of the Entity animation

	int	animationCounter,
		walkFrames,  // Number of walking sprites
		walkCounter,  // Number of walking sprites
		idleFrames, // Number of idleling sprites
		idleCounter, // Number of idleling sprites
		walkSpeed,
		idleSpeed;

	int lastMovement;

	/* Constructor
	*/
	PlayerEntity(sf::Texture& t) : Sprite() {
		speedBoost = 5;
		velocity.x = 0;
		velocity.y = 0;

		// Inicialize sprite counters
		walkCounter = 0;
		walkFrames = 2;
		idleCounter = 0;
		idleFrames = 1;
		walkSpeed = 8;
		idleSpeed = 10;

		lastMovement = 0;

		frameSize.x = 51;
		frameSize.y = 75;

		// Get texture sprites:
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 3; x++) {
				sf::IntRect frame(15 + frameSize.x * x, 24 + frameSize.y * y, frameSize.x, frameSize.y);
				frames.push_back(frame);
			}
		}


		setTextureRect(frames[0]);
		setTexture(t);
	}

	void animate(sf::Vector2f velocity) {
		if (velocity.x == 0 && velocity.y == 0) {
			setTextureRect(frames[lastMovement * 3]);
		}
		else {
			animationCounter %= walkSpeed;
			if (animationCounter == 0) {
				walkCounter = (walkCounter + 1) % walkFrames;
				setTextureRect(frames[(1 + walkCounter) + 3 * lastMovement]);
			}
		}
		animationCounter++;
	}

	/*
	 * Update player position
	 */
	void update(bool playerDown, bool playerUp, bool playerLeft, bool playerRight) {
		velocity.x = 0;
		velocity.y = 0;

		if (playerDown) {
			velocity.x = speedBoost;
			lastMovement = 0;
		}
		if (playerUp) {
			velocity.x = -speedBoost;
			lastMovement = 1;
		}
		if (playerLeft) {
			velocity.y = -speedBoost;
			lastMovement = 2;
		}
		if (playerRight) {
			velocity.y = speedBoost;
			lastMovement = 3;
		}

		animate(velocity);

		// Move Entity position
		move(velocity.y, velocity.x);
	}
};


int main(int argc, char* argv[]) {

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(windowsWidth, windowsHeight), "Bomberman");
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