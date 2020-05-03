#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameSounds.h"
#include "../GameEngine.hpp"
#include "GameInterfaceController.h"


class OptionsMenu {
	GameGUI::Menu* menu;

public:
	static GameInterfaceController::GameState lastGameStateOptionsMenu;
private:
	enum ButtonActions {
		AUDIO,
		GRAPHICS,
		CONTROLS,

		MASTER_VOLUME_SLIDER,
		MUSIC_SLIDER,
		SOUND_SLIDER,

		RESOLUTION,
		FULLSCREEN,
		FPS,

		SAVE_AND_QUIT,
		SAVE,
		QUIT
	};

	sf::Texture texture;
	sf::Sprite background;
	sf::RectangleShape menuBackground;
	sf::RectangleShape menuBackgroundShadow;
	sf::RectangleShape menuBackgroundShadow1;
	sf::RectangleShape menuBackgroundShadow2;

	GameGUI::Slider* masterVolumenSlider; GameGUI::Label* masterVText;
	GameGUI::Slider* musicSlider; GameGUI::Label* musicText;
	GameGUI::Slider* soundSlider; GameGUI::Label* soundText;

	GameGUI::OptionsBox<sf::Vector2i>* opt;
	GameGUI::Slider* fpsSlider; GameGUI::Label* fpsText;

private:
	void createBackgroundMenu(sf::RenderWindow& window, bool addButton = true) {
		if (addButton) {
			GameGUI::HorizontalBoxLayout* hbox2 = menu->addHorizontalBoxLayout();

			//hbox2->addButton("Aplicar y salir", ButtonActions::SAVE_AND_QUIT);
			//hbox2->addButton("Aplicar", ButtonActions::SAVE);
			hbox2->addButton("Atras", ButtonActions::QUIT);
		}

		menu->setPosition(sf::Vector2f((int)window.getSize().x / 2 - (int)menu->getSize().x / 2, (int)window.getSize().y / 2 - (int)menu->getSize().y / 2));

		float menuBackgroundPadding = 50;
		menuBackground.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding, menu->getSize().y + 2 * menuBackgroundPadding));
		menuBackground.setPosition(menu->getPosition().x - menuBackgroundPadding, menu->getPosition().y - menuBackgroundPadding);
		menuBackground.setFillColor(sf::Color(255, 255, 153, 200));

		menuBackgroundShadow.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding - 4, menu->getSize().y + 2 * menuBackgroundPadding - 4));
		menuBackgroundShadow.setPosition(menu->getPosition().x - menuBackgroundPadding + 12, menu->getPosition().y - menuBackgroundPadding + 12);
		menuBackgroundShadow.setFillColor(sf::Color(15, 35, 35, 60));

		menuBackgroundShadow1.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding - 2, menu->getSize().y + 2 * menuBackgroundPadding - 2));
		menuBackgroundShadow1.setPosition(menu->getPosition().x - menuBackgroundPadding + 11, menu->getPosition().y - menuBackgroundPadding + 11);
		menuBackgroundShadow1.setFillColor(sf::Color(15, 35, 35, 20));

		menuBackgroundShadow2.setSize(sf::Vector2f(menu->getSize().x + 2 * menuBackgroundPadding, menu->getSize().y + 2 * menuBackgroundPadding));
		menuBackgroundShadow2.setPosition(menu->getPosition().x - menuBackgroundPadding + 10, menu->getPosition().y - menuBackgroundPadding + 10);
		menuBackgroundShadow2.setFillColor(sf::Color(15, 35, 35, 20));
	}

	void createAudioMenu(sf::RenderWindow& window) {
		menu = new GameGUI::Menu(window);

		GameGUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();
		// GameGUI::VerticalBoxLayout* vbox = menu->addVerticalBoxLayout();

		GameGUI::HorizontalBoxLayout* masterLine = new GameGUI::HorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* musicLine = new GameGUI::HorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* soundLine = new GameGUI::HorizontalBoxLayout();

		hbox->addButton("Audio", ButtonActions::AUDIO);
		hbox->addButton("Graficos", ButtonActions::GRAPHICS);
		hbox->addButton("Controles", ButtonActions::CONTROLS);
		
		GameGUI::FormLayout *f = menu->addFormLayout();

		masterVolumenSlider = new GameGUI::Slider();
		masterVolumenSlider->setQuantum(2);
		masterVolumenSlider->setValue(GameMusic::getMasterVolume());

		masterLine->add(masterVolumenSlider, ButtonActions::MASTER_VOLUME_SLIDER);
		masterVText = masterLine->addLabel(to_string(GameMusic::getMasterVolume()));

		f->addRow("Volumen Maestro", masterLine);

		musicSlider = new GameGUI::Slider();
		musicSlider->setQuantum(2);
		musicSlider->setValue(GameMusic::getVolume());

		musicLine->add(musicSlider, ButtonActions::MUSIC_SLIDER);
		musicText = musicLine->addLabel(to_string(GameMusic::getVolume()));
		f->addRow("Musica", musicLine);

		soundSlider = new GameGUI::Slider();
		soundSlider->setQuantum(2);
		soundSlider->setValue(GameSounds::getVolume());

		soundLine->add(soundSlider, ButtonActions::SOUND_SLIDER);
		soundText = soundLine->addLabel(to_string(GameSounds::getVolume()));
		f->addRow("Sonido", soundLine);

		createBackgroundMenu(window);
	}

	void createGraphicsMenu(sf::RenderWindow& window) {
		menu = new GameGUI::Menu(window);

		GameGUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();
		GameGUI::FormLayout* f = menu->addFormLayout();

		hbox->addButton("Audio", ButtonActions::AUDIO);
		hbox->addButton("Graficos", ButtonActions::GRAPHICS);
		hbox->addButton("Controles", ButtonActions::CONTROLS);

		masterVolumenSlider = new GameGUI::Slider();
		masterVolumenSlider->setQuantum(4);
		masterVolumenSlider->setValue(GameMusic::getMasterVolume());
		//f->addRow("Resolucion      ", masterVolumenSlider, ButtonActions::MASTER_VOLUME_SLIDER);

		opt = new GameGUI::OptionsBox<sf::Vector2i>();
		opt->addItem("1024 x 768", sf::Vector2i(1024, 768));
		opt->addItem("1280 x 1024", sf::Vector2i(1280, 1024));
		opt->addItem("1280 x 800", sf::Vector2i(1280, 800));
		opt->addItem("1366 x 768", sf::Vector2i(1366, 768));
		opt->addItem("1920 x 1080", sf::Vector2i(1920, 1080));
		f->addRow("Resolucion", opt, ButtonActions::RESOLUTION);

		GameGUI::CheckBox* terminateProgram = new GameGUI::CheckBox();
		f->addRow("Pantalla Completa    ", terminateProgram, ButtonActions::FULLSCREEN);

		fpsSlider = new GameGUI::Slider();
		fpsSlider->setQuantum(2);
		fpsSlider->setValue(GameSounds::getVolume());
		
		GameGUI::HorizontalBoxLayout* fpsLine = new GameGUI::HorizontalBoxLayout();
		fpsLine->add(fpsSlider, ButtonActions::FPS);
		fpsText = fpsLine->addLabel(to_string(GameSounds::getVolume()));
		f->addRow("FPS", fpsLine);

		createBackgroundMenu(window);
	}

public:
	OptionsMenu(sf::RenderWindow& window) {
		texture.loadFromFile("../textures/interface/Background_orange_squares.png");
		texture.setRepeated(true);

		background.setTexture(texture);
		background.setColor(sf::Color(255, 255, 0, 5));
		background.setScale(sf::Vector2f(2, 2));
		background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });

		createAudioMenu(window);
	}

private:


	void userActions(sf::RenderWindow* &window, GameInterfaceController& gameDisplay, Game game) {
		sf::Event event;
		while (window->pollEvent(event)) {
			// Process events
			switch (event.type) {
				// window closed
			case sf::Event::Closed:
				// Close window -> exit
				window->close();
				break;
			case sf::Event::LostFocus:
				// Pause
				break;
			case sf::Event::GainedFocus:
				// Resume
				break;
			case sf::Event::Resized: {
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window->setView(sf::View(visibleArea));
				createBackgroundMenu(*window, false);
				break;
			}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//													BUTTON PRESSED
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
				int id = menu->onEvent(event);
				switch (id) {
				case ButtonActions::AUDIO:
					delete(menu);
					createAudioMenu(*window);
					break;
				case ButtonActions::GRAPHICS:
					delete(menu);
					createGraphicsMenu(*window);
					break;
				case ButtonActions::CONTROLS:
					break;
				case ButtonActions::MASTER_VOLUME_SLIDER:
					GameMusic::setMasterVolume(masterVolumenSlider->getValue());
					masterVText->setText(to_string(masterVolumenSlider->getValue()));
					break;
				case ButtonActions::MUSIC_SLIDER:
					GameMusic::setVolume(musicSlider->getValue());
					musicText->setText(to_string(musicSlider->getValue()));
					break;
				case ButtonActions::SOUND_SLIDER:
					GameSounds::setVolume(soundSlider->getValue());
					soundText->setText(to_string(soundSlider->getValue()));
					break;
				case ButtonActions::RESOLUTION: {
					/*sf::FloatRect visibleArea(0, 0, opt->getSelectedValue().x, opt->getSelectedValue().y);
					window.setView(sf::View(visibleArea));*/
					cout << opt->getSelectedValue().x << " x " << opt->getSelectedValue().y << endl;
					window->setSize(sf::Vector2u(opt->getSelectedValue().x, opt->getSelectedValue().y));
					// createGraphicsMenu(window);
					break;
				}
				case ButtonActions::FULLSCREEN:
					window->close();
					window = new RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Bombermenaman", sf::Style::Fullscreen);
					break;
				case ButtonActions::FPS: {
					int fpss = fpsSlider->getValue() * 5 / 4 + 5;
					if (fpss == 130) {
						fpsText->setText("MAX");
						//GameInterfaceController::FPSs = 0;
						fpss = 0;
					}
					else {
						fpsText->setText(to_string(fpss));
					}
					window->setFramerateLimit(fpss);

					break;
				}
				case ButtonActions::QUIT:
					gameDisplay.setGameState(lastGameStateOptionsMenu);
					break;
				}
			}
		}
	}

	void draw(sf::RenderWindow& window) {
		window.draw(background);

		window.draw(menuBackgroundShadow2);
		window.draw(menuBackgroundShadow1);
		window.draw(menuBackgroundShadow);
		window.draw(menuBackground);

		window.draw(*menu);
	}

public:
	void menuActions(GameInterfaceController& gameDisplay, Game game) {
		userActions(gameDisplay.getWindow(), gameDisplay, game);
		draw(*gameDisplay.getWindow());
	}
};


GameInterfaceController::GameState OptionsMenu::lastGameStateOptionsMenu;