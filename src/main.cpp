#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "GameEngine.hpp"
#include "Logic/Time.h"
#include "Logic/Random.h"

#include "Interface/GUI/GameGUI.hpp"
#include "Interface/GUI/Theme.hpp"

#include "Interface/GameInterfaceController.h"
#include "Interface/MainMenu.h"
#include "Interface/OptionsMenu.h"
#include "Interface/PauseMenu.h"


int main(int argc, char* argv[]) {
	// Start counting the time:
	GameTime::startGameTime();

	// Start Random Generator
	Random::initilizeRandomGen();

	// Create new game
	Game game = Game();

	// Create a Multiple interface controller
	auto gameDisplayController = GameInterfaceController();

	// Play Title music
	GameMusic::playTitleMusic();

	MainMenu gameMainMenu(*gameDisplayController.getWindow());
	OptionsMenu optionsMenu(*gameDisplayController.getWindow());
	PauseMenu pauseMenu(*gameDisplayController.getWindow());
	GameInterface gameInterface;

	// Start game loop
	while (gameDisplayController.windowOpen()) {
		switch (gameDisplayController.gameState) {
		case GameInterfaceController::GameState::MAIN_MENU:
			gameMainMenu.menuActions(gameDisplayController, game);
			break;

		case GameInterfaceController::GameState::OPTIONS_MENU:
			optionsMenu.menuActions(gameDisplayController, game);
			break;

		case GameInterfaceController::GameState::PAUSE_MENU:
			pauseMenu.menuActions(gameDisplayController, game);
			pauseMenu.checkUserPauseActions(gameDisplayController);
			break;

		case GameInterfaceController::GameState::PLAYING:
			game.update();

			// Clear screen from previous drawings
			gameDisplayController.getWindow()->clear();
			// Draw the player and the scene
			game.draw(*gameDisplayController.getWindow());
			/*for(Player_ptr &player : PLayers::getVectorPlayer()){
				playersLives.push_back(player->getLives());
				cout<<playersLives.front();
			}*/
			gameInterface.update(GameTime::getTimeNow());
			if (game.gameOptions.multiplayerGame) {
				//gameI.update(time.getTimeNow(),playersLives.front(),playersLives.back());
				gameInterface.drawMulti(*gameDisplayController.getWindow());
			}
			else {
				gameInterface.draw(*gameDisplayController.getWindow());
			}

			sf::Event event;
			while (gameDisplayController.getWindow()->pollEvent(event)) {
				// Process events
				switch (event.type) {
					// window closed
				case sf::Event::Closed:
					// Close window -> exit
					gameDisplayController.getWindow()->close();
					break;
				case sf::Event::LostFocus:
					// Pause
					break;
				case sf::Event::GainedFocus:
					// Resume
					break;
				}
			}

			pauseMenu.checkUserPauseActions(gameDisplayController);
		}

		// Update display window window
		gameDisplayController.getWindow()->display();
	}
}