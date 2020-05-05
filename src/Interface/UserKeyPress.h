#pragma once
#include "GameDisplayController.h"
#include "../Logic/Time.h"

/*
Key binding tutorial from SFML https://github.com/SFML/SFML/wiki/Tutorial:-Manage-dynamic-key-binding
 */
class UserKeyPress{

	enum InputType {
		KeyboardInput,
		MouseInput,
		JoystickInput
	};

	struct MyKeys {
		InputType myInputType;
		sf::Event::EventType myEventType;
		sf::Keyboard::Key myKeyCode;
		sf::Mouse::Button myMouseButton;
	};

	std::map<std::string, MyKeys> Keys;
	

	bool EsqPressed = false;
public:
	UserKeyPress() {
		MyKeys key;

		// Let's bind the left mouse button to the "Shoot" action
		key.myInputType = MouseInput;
		key.myEventType = sf::Event::MouseButtonPressed;
		key.myMouseButton = sf::Mouse::Left;
		Keys["Shoot"] = key;

		// Let's bind the Return key to the "Jump" action
		key.myInputType = KeyboardInput;
		key.myEventType = sf::Event::KeyPressed;
		key.myKeyCode = sf::Keyboard::Return;
		Keys["Jump"] = key;

		// Let's bind the Left Control key to the "Use" action
		key.myInputType = KeyboardInput;
		key.myEventType = sf::Event::KeyPressed;
		key.myKeyCode = sf::Keyboard::LControl;
		Keys["Use"] = key;
	}

private:
	bool TestEvent(MyKeys k, sf::Event e) {
		// Mouse event
		if (k.myInputType == MouseInput &&
			k.myEventType == e.type &&
			k.myMouseButton == e.mouseButton.button) {
			return (true);
		}
		// Keyboard event
		if (k.myInputType == KeyboardInput &&
			k.myEventType == e.type &&
			k.myKeyCode == e.key.code) {

			
			return (true);
		}
		return (false);
	}

	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		if (TestEvent(Keys["Shoot"], event)) {
			// You can use a function
			std::cout << "Shoot !" << std::endl;
		}
		if (TestEvent(Keys["Jump"], event)) {
			std::cout << "Jump !" << std::endl;
		}
		if (TestEvent(Keys["Use"], event)) {
			// or only code
			std::cout << "Use !" << std::endl;
		}
	}

public:
	void checkUserPauseActions(GameDisplayController& gameDisplay) {
		if (gameDisplay.getGameState() == GameDisplayController::GameState::PLAYING || gameDisplay.getGameState() == GameDisplayController::GameState::PAUSE_MENU) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				if (!EsqPressed) {
					EsqPressed = true;
					if (gameDisplay.getGameState() == GameDisplayController::GameState::PAUSE_MENU) {
						GameTime::resumeGameTime();
						gameDisplay.setGameState(GameDisplayController::GameState::PLAYING);
					}
					else {
						GameTime::stopGameTime();
						gameDisplay.setGameState(GameDisplayController::GameState::PAUSE_MENU);
					}
				}
			}
			else {
				EsqPressed = false;
			}
		}
	}

	void checkUserKeysPress(GameDisplayController& gameDisplay, Game& game){
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&UserKeyPress::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
	}
};