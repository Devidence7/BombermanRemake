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
#include "Interface/MultiplayerMenu.h"
#include "Interface/StoryModeMenu.h"
#include "Interface/DifficultMenu.h"
#include "Interface/LoadingScreen.h"
#include "Interface/UserKeyPress.h"
#include <unistd.h>


int main(int argc, char* argv[]) {
	// Start counting the time:
	GameTime::startGameTime();

	// Start Random Generator
	Random::initilizeRandomGen();

	// Create new game
	Game game = Game();
	

	// Create a Multiple interface controller
	auto gameDisplayController = GameDisplayController();
	LoadingScreen ls =LoadingScreen(*gameDisplayController.getWindow());

	// Play Title music
	GameMusic::playTitleMusic();

	MainMenu gameMainMenu(*gameDisplayController.getWindow());
	OptionsMenu optionsMenu(*gameDisplayController.getWindow());
	PauseMenu pauseMenu(*gameDisplayController.getWindow());
	GameOver gameOverMenu(*gameDisplayController.getWindow());
	MultiplayerMenu multiplayerMenu(*gameDisplayController.getWindow());
	StoryModeMenu storyModeMenu(*gameDisplayController.getWindow());
	DifficultyMenu difficultyMenu(*gameDisplayController.getWindow());
	GameInterface gameInterface(*gameDisplayController.getWindow());

	// Start game loop
	while (gameDisplayController.windowOpen()) {
		GameTime::updateCurrentTime();

		switch (gameDisplayController.gameState) {
		case GameDisplayController::GameState::MAIN_MENU:
		
			gameMainMenu.menuActions(gameDisplayController, game);
			break;

		case GameDisplayController::GameState::LOADING:
			//Mostrar pantalla de carga
			
			game.startNewGame(*gameDisplayController.getWindow(),gameDisplayController);
			
			gameInterface.iniPlayers();
			ls.draw(*gameDisplayController.getWindow());
			gameDisplayController.setGameState(GameDisplayController::GameState::PLAYING);
			break;
		
		case GameDisplayController::GameState::VS_NUMPLAYERS_MENU:
			multiplayerMenu.menuActions(gameDisplayController, game);
			break;


		case GameDisplayController::GameState::DIFFICULTY:
			difficultyMenu.menuActions(gameDisplayController, game);
			break;
		
		case GameDisplayController::GameState::STORY_MENU:
			storyModeMenu.menuActions(gameDisplayController, game);
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

		case GameDisplayController::GameState::PLAYING:
		
			game.update(gameDisplayController);
		
			// Clear screen from previous drawings
			gameDisplayController.getWindow()->clear();

			// Draw the player and the scene
			game.draw(*gameDisplayController.getWindow());
		
			gameInterface.update(GameTime::getTimeNow());
			
			gameInterface.draw(*gameDisplayController.getWindow());
			//}

			gameDisplayController.manageGameInterface(gameDisplayController);
			pauseMenu.checkUserPauseActions(gameDisplayController);
		}

		// Update display window window
		gameDisplayController.getWindow()->display();
	}
}