#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "GameEngine.hpp"
#include "Logic/Time.h"
#include "Logic/Random.h"
#include "Interface/MainMenu.h"
#include "Music/GameMusic.h"

#include "Interface/GUI/GameGUI.hpp"
#include "Interface/GUI/Theme.hpp"
#include "Interface/OptionsMenu.h"
#include "Interface/PauseMenu.h"

//#include "Map/Map.hpp"

int windowsHeight = 600;
int windowsWidth = 800;
bool primero = true;
bool multi = false;
int numPlayers;
list<int> playersLives;

GameInterface::GameState gameState;

int main(int argc, char* argv[]) {

	// Create the main window
	//unsigned int desktopWidth = sf::VideoMode::getDesktopMode().width;
	//unsigned int desktopHeight = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(27 * 48, 17 * 48), "Bombermenaman"); // , sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen

	// Start counting the time:
	GameTime::startGameTime();

	// Start Random Generator
	Random::initilizeRandomGen();

	Game game = Game();

	// Set the maximun Framerate Limit.
	window.setFramerateLimit(game.FPS);

	MainMenu gameMainMenu(window);
	OptionsMenu optionsMenu(window);
	PauseMenu pauseMenu(window);
	GameInterface gameI = GameInterface();
	GameMusic::playTitleMusic();

	// Start game loop
	while (window.isOpen()) {
		// Update time:
		GameTime::updateCurrentTime();

		switch (gameState) {
		case GameInterface::GameState::MAIN_MENU:
			gameMainMenu.menuActions(window, gameState, game);
			break;

		case GameInterface::GameState::OPTIONS_MENU:
			optionsMenu.menuActions(window, gameState, game);
			break;

		case GameInterface::GameState::PAUSE_MENU:
			pauseMenu.menuActions(window, gameState, game);
			pauseMenu.checkUserPauseActions(gameState);
			break;

		case GameInterface::GameState::PLAYING:
			// TODO PLAYER MOVEMENT MUST NOT DEPEND ON PROCESSOR SPEED THIS IS SHIIIIIIIIT
			game.update();

			// Clear screen from previous drawings
			window.clear();
			// Draw the player and the scene
			game.draw(window);
			/*for(Player_ptr &player : PLayers::getVectorPlayer()){
				playersLives.push_back(player->getLives());
				cout<<playersLives.front();
			}*/
			gameI.update(GameTime::getTimeNow());
			if (game.gameOptions.multiplayerGame) {
				//gameI.update(time.getTimeNow(),playersLives.front(),playersLives.back());
				gameI.drawMulti(window);
			}
			else {
				gameI.draw(window);
			}

			pauseMenu.checkUserPauseActions(gameState);
		}

		// Update window
		window.display();
	}
}