#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "GameEngine.hpp"
#include "Logic/Time.h"
#include "Logic/Random.h"

#include "Interface/GUI/GameGUI.hpp"
#include "Interface/GUI/Theme.hpp"
//#include "Interface/GameDisplayController.h"
#include "Interface/MainMenu.h"
#include "Interface/OptionsMenu.h"
#include "Interface/PauseMenu.h"
#include "Interface/GameOver.h"
#include "Interface/MultiplayerMenu.h"
#include "Interface/StoryModeMenu.h"
#include "Interface/DifficultMenu.h"
#include "Interface/LoadingScreen.h"
#include "Interface/VictoryScreen.h"
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
	LoadingScreen ls =LoadingScreen(*gameDisplayController.getWindow(),game);

	// Play Title music
	GameMusic::playTitleMusic();

	MainMenu gameMainMenu(*gameDisplayController.getWindow());
	OptionsMenu optionsMenu(*gameDisplayController.getWindow());
	PauseMenu pauseMenu(*gameDisplayController.getWindow());
	GameOver gameOverMenu(*gameDisplayController.getWindow());
	MultiplayerMenu multiplayerMenu(*gameDisplayController.getWindow(),gameDisplayController);
	StoryModeMenu storyModeMenu(*gameDisplayController.getWindow());
	DifficultyMenu difficultyMenu(*gameDisplayController.getWindow());
	GameInterface gameInterface(*gameDisplayController.getWindow());
	VictoryScreen victory(*gameDisplayController.getWindow(),game);

	// Start game loop
	while (gameDisplayController.windowOpen()) {
		GameTime::updateCurrentTime();

		switch (gameDisplayController.gameState) {
		case GameDisplayController::GameState::MAIN_MENU:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			gameMainMenu.menuActions(gameDisplayController, game);
			//victory.menuActions(gameDisplayController, game);
			break;

		case GameDisplayController::GameState::VICTORY:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			victory.menuActions(gameDisplayController, game);
			break;

		case GameDisplayController::GameState::LOADING:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			//Mostrar pantalla de carga
			cout<<"Nivel: "<<game.stage<<endl;
			game.startNewGame(*gameDisplayController.getWindow(),gameDisplayController);
			
			gameInterface.createInterface(*gameDisplayController.getWindow());
			
			gameDisplayController.setGameState(GameDisplayController::GameState::PLAYING);
			break;
		
		case GameDisplayController::GameState::VS_NUMPLAYERS_MENU:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			multiplayerMenu.menuActions(gameDisplayController, game);
			break;

		
		case GameDisplayController::GameState::STORY_MENU:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			storyModeMenu.menuActions(gameDisplayController, game);
			break;


		case GameDisplayController::GameState::OPTIONS_MENU:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			optionsMenu.menuActions(gameDisplayController, game);
			break;

		case GameDisplayController::GameState::PAUSE_MENU:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			pauseMenu.menuActions(gameDisplayController, game);
			pauseMenu.checkUserPauseActions(gameDisplayController);
			break;

		case GameDisplayController::GameState::GAME_OVER:
			gameDisplayController.getWindow()->setView(gameDisplayController.menuView);
			gameOverMenu.menuActions(gameDisplayController, game);
			break;


		case GameDisplayController::GameState::PLAYING:

			// Clear screen from previous drawings
			gameDisplayController.getWindow()->clear(sf::Color(0, 102, 128));
		
			game.update(gameDisplayController);
		
			// Draw the player and the scene
			game.draw(*gameDisplayController.getWindow());
			
			// Manage user window actions
			gameDisplayController.manageGameInterface(gameDisplayController);

			gameInterface.update();

			gameInterface.draw(gameDisplayController);

			// Manage pause menu when playing
			pauseMenu.checkUserPauseActions(gameDisplayController);
		}



		// Update display window window
		gameDisplayController.getWindow()->display();
	}
}