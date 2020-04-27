#pragma once
#include <SFML/Graphics.hpp>
#include "InterfacePaths.h"
#include "../Include/global.hpp"
#include <iostream>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#define MAX_NUMBER_OF_ITEMS 3
using namespace std;
using namespace GameGUI;

class PauseMenu {	
	GameGUI::Menu* menu;

	enum ButtonActions {
		AUDIO,
		GRAPHICS,
		CONTROLS,

		MASTER_VOLUME_SLIDER,
		MUSIC_SLIDER,
		SOUND_SLIDER,

		SAVE_AND_QUIT,
		SAVE,
		QUIT
	};

	sf::Texture texture;
	sf::Sprite background;

	GameGUI::Slider* masterVolumenSlider;
		GameGUI::Slider* musicSlider;

public:
	PauseMenu(sf::RenderWindow& window){
		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);

		background.setTexture(texture);
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y});

		menu = new GameGUI::Menu(window);

		GameGUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();
		GameGUI::FormLayout* f = menu->addFormLayout();
		GameGUI::HorizontalBoxLayout* hbox2 = menu->addHorizontalBoxLayout();

		hbox->addButton("Audio", ButtonActions::AUDIO);
		hbox->addButton("Graficos", ButtonActions::GRAPHICS);
		hbox->addButton("Controles", ButtonActions::CONTROLS);

		masterVolumenSlider = new GameGUI::Slider();
		masterVolumenSlider->setQuantum(1);
		masterVolumenSlider->setValue(GameMusic::masterVolume);
		f->addRow("Master Volume", masterVolumenSlider, ButtonActions::MASTER_VOLUME_SLIDER);

		musicSlider = new GameGUI::Slider();
		musicSlider->setQuantum(1);
		musicSlider->setValue(GameMusic::getVolume());
		f->addRow("Music", musicSlider, ButtonActions::MUSIC_SLIDER);
		f->addRow("Sound", new GameGUI::Slider(), ButtonActions::SOUND_SLIDER);

		//hbox2->addButton("Aplicar y salir", ButtonActions::SAVE_AND_QUIT);
		//hbox2->addButton("Aplicar", ButtonActions::SAVE);
		hbox2->addButton("Atras", ButtonActions::QUIT);


		menu->setPosition(sf::Vector2f((int)window.getSize().x/2 - (int)menu->getSize().x/2, (int)window.getSize().y / 2 - (int)menu->getSize().y / 2));
    }

private:
	void userActions(sf::RenderWindow& window, GameInterface::GameState& gameState, Game game) {
		sf::Event event;
		while (window.pollEvent(event)) {
			// Process events
			switch (event.type) {
				// window closed
			case sf::Event::Closed:
				// Close window -> exit
				window.close();
				break;
			case sf::Event::LostFocus:
				// Pause
				break;
			case sf::Event::GainedFocus:
				// Resume
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//													BUTTON PRESSED
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
				int id = menu->onEvent(event);
				switch (id) {
				case ButtonActions::AUDIO:
					break;
				case ButtonActions::GRAPHICS:
					break;
				case ButtonActions::CONTROLS:
					break;
				case ButtonActions::MASTER_VOLUME_SLIDER:
					GameMusic::masterVolume = masterVolumenSlider->getValue();
					GameMusic::updateVolumen();
					break;
				case ButtonActions::MUSIC_SLIDER:
					GameMusic::setVolume(musicSlider->getValue());
					break;
				case ButtonActions::SOUND_SLIDER:
					break;
				case ButtonActions::QUIT:
					gameState = GameInterface::GameState::MAIN_MENU;
					break;
				}
			}
		}
	}

	void draw(sf::RenderWindow& window) {	
		window.draw(background);
		window.draw(*menu);
	}

public:
	void menuActions(sf::RenderWindow& window, GameInterface::GameState& gameState, Game game) {
		userActions(window, gameState, game);
		draw(window);
	}
};
