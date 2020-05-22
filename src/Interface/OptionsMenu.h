#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/Theme.hpp"
#include "GUI/GameGUI.hpp"
#include "../Music/GameMusic.h"
#include "../GameEngine.hpp"
#include "GameDisplayController.h"


class OptionsMenu {
	GameGUI::Menu* menu;

public:
	static GameDisplayController::GameState lastGameStateOptionsMenu;
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

		PLAYER1_CONTROLS,
		PLAYER2_CONTROLS,

		P1_UP,
		P1_DOWN,
		P1_LEFT,
		P1_RIGHT,
		P1_BOMB,
		P1_ACTION,

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
	GameGUI::CheckBox* fullScreenCheckBox;
	GameGUI::Slider* fpsSlider; GameGUI::Label* fpsText;

	bool waitingForKey = false;
	int waitingForKeyPlayer = 1;
	ButtonActions waitingForKeyKey;

	double lastBlickTime = 0;
	double blickSpeed = 0.5;
	bool isLetterInvisible = false;

	GameGUI::HorizontalBoxLayout* hboxQuit = nullptr;
	GameGUI::FormLayout* f_Controls;
	GameGUI::HorizontalBoxLayout* hbox_Controls;
	GameGUI::HorizontalBoxLayout* hbox2_Controls;

	GameGUI::Label* playerLabel;
	GameGUI::Button* b_up;
	GameGUI::Button* b_down;
	GameGUI::Button* b_left;
	GameGUI::Button* b_right;
	GameGUI::Button* b_bomb;
	GameGUI::Button* b_action;

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
	void centerElement(GameGUI::Widget* e) {
		sf::Vector2f eSize = e->getSize();
		sf::Vector2f mSize = menu->getSize();
		e->setPosition(e->getPosition() + sf::Vector2f(mSize.x / 2 - eSize.x / 2, 0));
	}

	void endLineElement(GameGUI::Widget* e) {
		sf::Vector2f eSize = e->getSize();
		sf::Vector2f mSize = menu->getSize();
		e->setPosition(e->getPosition() + sf::Vector2f(mSize.x - eSize.x, 0));
	}

	void correctControlsMenu() {
		endLineElement(hboxQuit);
		centerElement(hbox_Controls);
		centerElement(hbox2_Controls);
		centerElement(f_Controls);
	}

	void createBackgroundMenu(sf::RenderWindow& window, bool addButton = true) {
		if (addButton) {
			hboxQuit = menu->addHorizontalBoxLayout();

			//hbox2->addButton("Aplicar y salir", ButtonActions::SAVE_AND_QUIT);
			//hbox2->addButton("Aplicar", ButtonActions::SAVE);
			hboxQuit->addButton("Atras", ButtonActions::QUIT);
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


	void createControlsMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);
		// GameGUI::VerticalBoxLayout* vbox = menu->addVerticalBoxLayout();

		hbox_Controls = menu->addHorizontalBoxLayout();
		// GameGUI::VerticalBoxLayout* vbox = menu->addVerticalBoxLayout();

		GameGUI::HorizontalBoxLayout* masterLine = new GameGUI::HorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* musicLine = new GameGUI::HorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* soundLine = new GameGUI::HorizontalBoxLayout();

		auto b1 = hbox_Controls->addButton("Audio", ButtonActions::AUDIO);
		b1->goDownDown = 2;
		b1->transformableOptions = false;
		auto b2 = hbox_Controls->addButton("Graficos", ButtonActions::GRAPHICS);
		b2->goUpUp = 2;
		b2->transformableOptions = false;
		hbox_Controls->add(new GameGUI::Label("Controles"));
		//hbox_Controls->addButton("Controles", ButtonActions::CONTROLS);

		hbox2_Controls = menu->addHorizontalBoxLayout();
		//hbox2_Controls->addButton("Player 1", ButtonActions::PLAYER1_CONTROLS);

		//hbox2_Controls->addButton("Player 2", ButtonActions::PLAYER2_CONTROLS);

		if (waitingForKeyPlayer == 1) {
			hbox2_Controls->add(new GameGUI::Label("Player 1"), ButtonActions::PLAYER1_CONTROLS);
			hbox2_Controls->add(new GameGUI::Button("Player 2"), ButtonActions::PLAYER2_CONTROLS);
		}
		else {
			hbox2_Controls->add(new GameGUI::Button("Player 1"), ButtonActions::PLAYER1_CONTROLS);
			hbox2_Controls->add(new GameGUI::Label("Player 2"), ButtonActions::PLAYER2_CONTROLS);
		}


		GameGUI::HorizontalBoxLayout* hbox3 = menu->addHorizontalBoxLayout();
		f_Controls = menu->addFormLayout();
		// playerLabel = new GameGUI::Label("Player 1");
		// f_Controls->add(playerLabel);

		if (waitingForKeyPlayer == 1) {
			b_up = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goUp));
			f_Controls->addRow("ARRIBA", b_up, ButtonActions::P1_UP);

			b_down = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goDown));
			f_Controls->addRow("ABAJO", b_down, ButtonActions::P1_DOWN);

			b_left = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goLeft));
			f_Controls->addRow("IZQUIERDA", b_left, ButtonActions::P1_LEFT);

			b_right = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goRight));
			f_Controls->addRow("DERECHA", b_right, ButtonActions::P1_RIGHT);

			b_bomb = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.UseBomb));
			f_Controls->addRow("Usar Bomba", b_bomb, ButtonActions::P1_BOMB);

			b_action = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.MakeAction));
			f_Controls->addRow("Ejecutar Accion", b_action, ButtonActions::P1_ACTION);
		}
		else {
			b_up = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goUp));
			f_Controls->addRow("ARRIBA", b_up, ButtonActions::P1_UP);

			b_down = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goDown));
			f_Controls->addRow("ABAJO", b_down, ButtonActions::P1_DOWN);

			b_left = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goLeft));
			f_Controls->addRow("IZQUIERDA", b_left, ButtonActions::P1_LEFT);

			b_right = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goRight));
			f_Controls->addRow("DERECHA", b_right, ButtonActions::P1_RIGHT);

			b_bomb = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.UseBomb));
			f_Controls->addRow("Usar Bomba", b_bomb, ButtonActions::P1_BOMB);

			b_action = new GameGUI::Button(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.MakeAction));
			f_Controls->addRow("Ejecutar Accion", b_action, ButtonActions::P1_ACTION);
		}

		createBackgroundMenu(window);
		correctControlsMenu();
		//centerElement(hbox);
		//centerElement(hbox2);
		//centerElement(f);
	}

	void createAudioMenu(sf::RenderWindow& window) {
		menu = new GameGUI::Menu(window);

		GameGUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();
		// GameGUI::VerticalBoxLayout* vbox = menu->addVerticalBoxLayout();

		GameGUI::HorizontalBoxLayout* masterLine = new GameGUI::HorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* musicLine = new GameGUI::HorizontalBoxLayout();
		GameGUI::HorizontalBoxLayout* soundLine = new GameGUI::HorizontalBoxLayout();

		// hbox->addButton("Audio", ButtonActions::AUDIO);
		hbox->add(new GameGUI::Label("AUDIO"));
		auto b2 = hbox->addButton("Graficos", ButtonActions::GRAPHICS);
		b2->goDownDown = 2;
		b2->transformableOptions = false;
		auto b3 = hbox->addButton("Controles", ButtonActions::CONTROLS);
		b3->goUpUp = 2;
		b3->transformableOptions = false;

		GameGUI::FormLayout* f = menu->addFormLayout();
		f->add(new GameGUI::Label(" "));

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
		endLineElement(hboxQuit);
		centerElement(hbox);
	}

	void createGraphicsMenu(sf::RenderWindow& window, GameDisplayController& gameDisplay) {
		menu = new GameGUI::Menu(window);

		GameGUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();
		GameGUI::FormLayout* f = menu->addFormLayout();
		f->add(new GameGUI::Label(" "));

		auto* b1 = hbox->addButton("Audio", ButtonActions::AUDIO);
		b1->goDownDown = 2;
		b1->transformableOptions = false;
		//hbox->addButton("Graficos", ButtonActions::GRAPHICS);
		hbox->add(new GameGUI::Label("Graficos"));
		auto b2 = hbox->addButton("Controles", ButtonActions::CONTROLS);
		b2->goUpUp = 2;
		b2->transformableOptions = false;

		//f->addRow("Resolucion      ", masterVolumenSlider, ButtonActions::MASTER_VOLUME_SLIDER);

		/*opt = new GameGUI::OptionsBox<sf::Vector2i>();
		opt->addItem("1024 x 768", sf::Vector2i(1024, 768));
		opt->addItem("1280 x 1024", sf::Vector2i(1280, 1024));
		opt->addItem("1280 x 800", sf::Vector2i(1280, 800));
		opt->addItem("1366 x 768", sf::Vector2i(1366, 768));
		opt->addItem("1920 x 1080", sf::Vector2i(1920, 1080));
		f->addRow("Resolucion", opt, ButtonActions::RESOLUTION);*/

		fullScreenCheckBox = new GameGUI::CheckBox(gameDisplay.fullScreen);
		f->addRow("Pantalla Completa    ", fullScreenCheckBox, ButtonActions::FULLSCREEN);

		fpsSlider = new GameGUI::Slider();
		fpsSlider->setQuantum(2);
		fpsSlider->setValue(gameDisplay.FPSs);

		GameGUI::HorizontalBoxLayout* fpsLine = new GameGUI::HorizontalBoxLayout();
		fpsLine->add(fpsSlider, ButtonActions::FPS);
		if (gameDisplay.FPSs) {
			fpsText = fpsLine->addLabel(to_string(gameDisplay.FPSs));
		}
		else {
			fpsText = fpsLine->addLabel("MAX");
			fpsSlider->setValue(100);
		}

		f->addRow("FPS", fpsLine);

		createBackgroundMenu(window);
		endLineElement(hboxQuit);
		centerElement(hbox);
	}

	void writeUserKeys(GameDisplayController& gameDisplay) {
		if (waitingForKeyPlayer == 1) {
			b_up->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goUp));
			b_down->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goDown));
			b_left->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goLeft));
			b_right->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goRight));
			b_bomb->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.UseBomb));
			b_action->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.MakeAction));
		}
		else {
			b_up->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goUp));
			b_down->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goDown));
			b_left->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goLeft));
			b_right->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goRight));
			b_bomb->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.UseBomb));
			b_action->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.MakeAction));
		}
	}

	void cleanUserKey(GameDisplayController& gameDisplay) {
		switch (waitingForKeyKey) {
		case P1_UP:
			b_up->setString(" ");
			break;
		case P1_DOWN:
			b_down->setString(" ");
			break;
		case P1_LEFT:
			b_left->setString(" ");
			break;
		case P1_RIGHT:
			b_right->setString(" ");
			break;
		case P1_BOMB:
			b_bomb->setString(" ");
			break;
		case P1_ACTION:
			b_action->setString(" ");
			break;
		}
	}

	bool checkIfKeyIsInUse(GameDisplayController& gameDisplay, sf::Keyboard::Key& key) {
		bool returnVal = false;

		if (key == gameDisplay.userKeyPress->player1.goUp && !(waitingForKeyPlayer == 1 && waitingForKeyKey == P1_UP)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player1.goDown && !(waitingForKeyPlayer == 1 && waitingForKeyKey == P1_DOWN)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player1.goLeft && !(waitingForKeyPlayer == 1 && waitingForKeyKey == P1_LEFT)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player1.goRight && !(waitingForKeyPlayer == 1 && waitingForKeyKey == P1_RIGHT)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player1.UseBomb && !(waitingForKeyPlayer == 1 && waitingForKeyKey == P1_BOMB)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player1.MakeAction && !(waitingForKeyPlayer == 1 && waitingForKeyKey == P1_ACTION)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player2.goUp && !(waitingForKeyPlayer == 2 && waitingForKeyKey == P1_UP)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player2.goDown && !(waitingForKeyPlayer == 2 && waitingForKeyKey == P1_DOWN)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player2.goLeft && !(waitingForKeyPlayer == 2 && waitingForKeyKey == P1_LEFT)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player2.goRight && !(waitingForKeyPlayer == 2 && waitingForKeyKey == P1_RIGHT)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player2.UseBomb && !(waitingForKeyPlayer == 2 && waitingForKeyKey == P1_BOMB)) {
			returnVal = true;
		}
		else if (key == gameDisplay.userKeyPress->player2.MakeAction && !(waitingForKeyPlayer == 2 && waitingForKeyKey == P1_ACTION)) {
			returnVal = true;
		}

		return returnVal;
	}

	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
		if (waitingForKey) {
			if (event.type == sf::Event::KeyPressed) {
				cout << gameDisplay.userKeyPress->getKeyName(event.key.code) << endl;
				if (checkIfKeyIsInUse(gameDisplay, event.key.code)) {
					GameSounds::buttonError();
				}
				else {
					switch (waitingForKeyKey) {
					case P1_UP:
						b_up->setString(gameDisplay.userKeyPress->getKeyName(event.key.code));
						if (waitingForKeyPlayer == 1) {
							gameDisplay.userKeyPress->player1.goUp = (sf::Keyboard::Key)event.key.code;
						}
						else {
							gameDisplay.userKeyPress->player2.goUp = (sf::Keyboard::Key)event.key.code;
						}
						break;
					case P1_DOWN:
						b_down->setString(gameDisplay.userKeyPress->getKeyName(event.key.code));
						if (waitingForKeyPlayer == 1) {
							gameDisplay.userKeyPress->player1.goDown = (sf::Keyboard::Key)event.key.code;
						}
						else {
							gameDisplay.userKeyPress->player2.goDown = (sf::Keyboard::Key)event.key.code;
						}
						break;
					case P1_LEFT:
						b_left->setString(gameDisplay.userKeyPress->getKeyName(event.key.code));
						if (waitingForKeyPlayer == 1) {
							gameDisplay.userKeyPress->player1.goLeft = (sf::Keyboard::Key)event.key.code;
						}
						else {
							gameDisplay.userKeyPress->player2.goLeft = (sf::Keyboard::Key)event.key.code;
						}
						break;
					case P1_RIGHT:
						b_right->setString(gameDisplay.userKeyPress->getKeyName(event.key.code));
						if (waitingForKeyPlayer == 1) {
							gameDisplay.userKeyPress->player1.goRight = (sf::Keyboard::Key)event.key.code;
						}
						else {
							gameDisplay.userKeyPress->player2.goRight = (sf::Keyboard::Key)event.key.code;
						}
						break;
					case P1_BOMB:
						b_bomb->setString(gameDisplay.userKeyPress->getKeyName(event.key.code));
						if (waitingForKeyPlayer == 1) {
							gameDisplay.userKeyPress->player1.UseBomb = (sf::Keyboard::Key)event.key.code;
						}
						else {
							gameDisplay.userKeyPress->player2.UseBomb = (sf::Keyboard::Key)event.key.code;
						}
						break;
					case P1_ACTION:
						b_action->setString(gameDisplay.userKeyPress->getKeyName(event.key.code));
						if (waitingForKeyPlayer == 1) {
							gameDisplay.userKeyPress->player1.MakeAction = (sf::Keyboard::Key)event.key.code;
						}
						else {
							gameDisplay.userKeyPress->player2.MakeAction = (sf::Keyboard::Key)event.key.code;
						}
						break;
					}
					GameSounds::buttonPress();
					writeUserKeys(gameDisplay);
					correctControlsMenu();
					waitingForKey = false;
				}
			}
		}

		int id = menu->onEvent(event);
		switch (id) {
		case ButtonActions::AUDIO:
			writeUserKeys(gameDisplay);
			correctControlsMenu();
			waitingForKey = false;
			delete(menu);
			createAudioMenu(*window);
			break;
		case ButtonActions::GRAPHICS:
			writeUserKeys(gameDisplay);
			correctControlsMenu();
			waitingForKey = false;
			delete(menu);
			createGraphicsMenu(*window, gameDisplay);
			break;
		case ButtonActions::CONTROLS:
			delete(menu);
			createControlsMenu(*window, gameDisplay);
			break;
		case ButtonActions::MASTER_VOLUME_SLIDER:
			GameMusic::setMasterVolume(masterVolumenSlider->getValue());
			masterVText->setText(to_string(masterVolumenSlider->getValue()), false);
			break;
		case ButtonActions::MUSIC_SLIDER:
			GameMusic::setVolume(musicSlider->getValue());
			musicText->setText(to_string(musicSlider->getValue()), false);
			break;
		case ButtonActions::SOUND_SLIDER:
			GameSounds::setVolume(soundSlider->getValue());
			soundText->setText(to_string(soundSlider->getValue()), false);
			GameSounds::soundChange();
			break;
		case ButtonActions::RESOLUTION: {
			/*sf::FloatRect visibleArea(0, 0, opt->getSelectedValue().x, opt->getSelectedValue().y);
			window.setView(sf::View(visibleArea));*/
			/*gameDisplay.windowWidth = opt->getSelectedValue().x;
			gameDisplay.windowHeight = opt->getSelectedValue().y;
			gameDisplay.notifyChangeDisplay();

			window->setView(gameDisplay.menuView);
			window->setSize(sf::Vector2u(opt->getSelectedValue().x, opt->getSelectedValue().y));
			gameDisplay.camera.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
			gameDisplay.playingGuiView = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y / 5));
			gameDisplay.playingGuiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.2f));

			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			gameDisplay.menuView = sf::View(visibleArea);*/
			// createGraphicsMenu(window);
			break;
		}
		case ButtonActions::FULLSCREEN:
			window->close();
			if (fullScreenCheckBox->isChecked()) {
				gameDisplay.fullScreen = true;
				window = new RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Bomberman", sf::Style::Fullscreen);
			}
			else {
				gameDisplay.fullScreen = false;
				window = new RenderWindow(sf::VideoMode(gameDisplay.windowWidth, gameDisplay.windowHeight), "Bomberman");
			}
			window->setFramerateLimit(gameDisplay.FPSs);

			// CAMERA
			gameDisplay.menuView = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
			gameDisplay.camera = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
			gameDisplay.camera.setViewport(sf::FloatRect(0.f, 0.1f, 1.f, 1.f));
			gameDisplay.playingGuiView = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y / 5));
			gameDisplay.playingGuiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.2f));
			window->setFramerateLimit(gameDisplay.FPSs);

			gameDisplay.notifyChangeDisplay();
			break;
		case ButtonActions::FPS: {
			int fpss = fpsSlider->getValue() * 5 / 4 + 5;
			if (fpss == 130) {
				fpsText->setText("MAX", false);
				//GameInterfaceController::FPSs = 0;
				fpss = 0;

			}
			else {
				fpsText->setText(to_string(fpss), false);
			}
			window->setFramerateLimit(fpss);
			gameDisplay.FPSs = fpss;

			break;
		}
		case ButtonActions::PLAYER1_CONTROLS:
			waitingForKeyPlayer = 1;
			//playerLabel->setText("Player 1");
			/*b_up->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goUp));
			b_down->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goDown));
			b_left->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goLeft));
			b_right->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.goRight));
			b_bomb->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.UseBomb));
			b_action->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player1.MakeAction));
			correctControlsMenu();*/
			createControlsMenu(*window, gameDisplay);
			break;
		case ButtonActions::PLAYER2_CONTROLS:

			waitingForKeyPlayer = 2;
			//playerLabel->setText("Player 2");
			/*b_up->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goUp));
			b_down->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goDown));
			b_left->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goLeft));
			b_right->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.goRight));
			b_bomb->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.UseBomb));
			b_action->setString(gameDisplay.userKeyPress->getKeyName(gameDisplay.userKeyPress->player2.MakeAction));
			correctControlsMenu();*/
			createControlsMenu(*window, gameDisplay);
			break;

		case ButtonActions::P1_UP:
			waitingForKey = true;
			waitingForKeyKey = P1_UP;
			break;
		case ButtonActions::P1_DOWN:
			waitingForKey = true;
			waitingForKeyKey = P1_DOWN;
			break;
		case ButtonActions::P1_LEFT:
			waitingForKey = true;
			waitingForKeyKey = P1_LEFT;
			break;
		case ButtonActions::P1_RIGHT:
			waitingForKey = true;
			waitingForKeyKey = P1_RIGHT;
			break;
		case ButtonActions::P1_BOMB:
			waitingForKey = true;
			waitingForKeyKey = P1_BOMB;
			break;
		case ButtonActions::P1_ACTION:
			waitingForKey = true;
			waitingForKeyKey = P1_ACTION;
			break;
		case ButtonActions::QUIT:
			gameDisplay.setGameState(lastGameStateOptionsMenu);
			gameDisplay.saveProperties();
			break;
		}

		if (id != -1 && id != MASTER_VOLUME_SLIDER && id != SOUND_SLIDER && id != MUSIC_SLIDER && id != FPS) {
			GameSounds::buttonPress();
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
	void menuActions(GameDisplayController& gameDisplay, Game& game) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			writeUserKeys(gameDisplay);
			correctControlsMenu();
			waitingForKey = false;
		}

		if (gameDisplay.optionsMenuReprocessDisplay) {
			gameDisplay.optionsMenuReprocessDisplay = false;
			createBackgroundMenu(*gameDisplay.getWindow(), false);
		}

		if (waitingForKey) {
			menu->userCanMakeActions = false;
			if (GameTime::getTimeNow() - lastBlickTime > blickSpeed) {
				if (isLetterInvisible) {
					writeUserKeys(gameDisplay);
				}
				else {
					cleanUserKey(gameDisplay);
				}
				correctControlsMenu();
				isLetterInvisible = !isLetterInvisible;
				lastBlickTime = GameTime::getTimeNow();
			}
		}
		else {
			menu->userCanMakeActions = true;
		}

		draw(*gameDisplay.getWindow());

		// Manage window events and pass a callback to manage this menu buttons
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&OptionsMenu::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
	}
};


GameDisplayController::GameState OptionsMenu::lastGameStateOptionsMenu;