#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "GameEngine.hpp"
#include "Logic/Time.h"
#include "Logic/Random.h"

#include "Interface/GUI/GameGUI.hpp"
#include "Interface/GUI/Theme.hpp"

#include "Interface/GameDisplayController.h"
#include "Interface/MainMenu.h"
#include "Interface/OptionsMenu.h"
#include "Interface/PauseMenu.h"
#include "Interface/GameOver.h"
#include "Interface/UserKeyPress.h"


int main(int argc, char* argv[]) {
	// Start counting the time:
	GameTime::startGameTime();

	// Start Random Generator
	Random::initilizeRandomGen();

	// Create new game
	Game game = Game();

	// Create a Multiple interface controller
	auto gameDisplayController = GameDisplayController();

	// Play Title music
	GameMusic::playTitleMusic();

	MainMenu gameMainMenu(*gameDisplayController.getWindow());
	OptionsMenu optionsMenu(*gameDisplayController.getWindow());
	PauseMenu pauseMenu(*gameDisplayController.getWindow());
	GameOver gameOverMenu(*gameDisplayController.getWindow());
	GameInterface gameInterface;
	UserKeyPress userKeyPressManager;

	// Start game loop
	while (gameDisplayController.windowOpen()) {
		GameTime::updateCurrentTime();

		switch (gameDisplayController.gameState) {
		case GameDisplayController::GameState::MAIN_MENU:
			gameMainMenu.menuActions(gameDisplayController, game);
			break;

		case GameDisplayController::GameState::OPTIONS_MENU:
			optionsMenu.menuActions(gameDisplayController, game);
			break;

		case GameDisplayController::GameState::PAUSE_MENU:
			pauseMenu.menuActions(gameDisplayController, game);
			pauseMenu.checkUserPauseActions(gameDisplayController);
			break;

		case GameDisplayController::GameState::GAME_OVER:
			gameOverMenu.menuActions(gameDisplayController, game);
			//gameOverMenu.checkUserPauseActions(gameDisplayController);
			break;

		case GameDisplayController::GameState::RESTART:
			game.restartGame(*gameDisplayController.getWindow());
			/*cout<<"holi5"<<endl;
			gameDisplayController.setGameState(GameDisplayController::GameState::PLAYING);
			cout<<"holi6"<<endl;*/
			break;

		case GameDisplayController::GameState::PLAYING:
			game.update(gameDisplayController);

			// Clear screen from previous drawings
			gameDisplayController.getWindow()->clear();
			// Draw the player and the scene
			game.draw(*gameDisplayController.getWindow());
			/*for(Player_ptr &player : PLayers::getVectorPlayer()){
				playersLives.push_back(player->getLives());
				cout<<playersLives.front();
			}*/
			gameInterface.update(GameTime::getTimeNow());
			/*if (game.gameOptions.multiplayerGame) {
				//gameI.update(time.getTimeNow(),playersLives.front(),playersLives.back());
				gameInterface.drawMulti(*gameDisplayController.getWindow());
			}*/
			//else {
			gameInterface.draw(*gameDisplayController.getWindow());
			//}

			gameDisplayController.manageGameInterface(gameDisplayController);
			pauseMenu.checkUserPauseActions(gameDisplayController);
			// userKeyPressManager.checkUserKeysPress(gameDisplayController, game);


			//gameOverMenu.checkUserGameOverActions(gameDisplayController);
		}

		// Update display window window
		gameDisplayController.getWindow()->display();
	}
}