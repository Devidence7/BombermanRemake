#pragma once

/*
Key binding tutorial from SFML https://github.com/SFML/SFML/wiki/Tutorial:-Manage-dynamic-key-binding
 */
class UserKeyPress{
	std::map<std::string, sf::Keyboard::Key> Keys;
	bool EsqPressed = false;

	PlayerEntity::PlayerControls player1;
	PlayerEntity::PlayerControls player2;

	// Get human redeable kay name
	std::string getKeyName(const int key) {
		if (key < 0 || key > 100) {
			cout << "Unknow new key: " + to_string(key) << endl;
			return "UNKOWN";
		}

		const static std::string keyNames[] = {

			"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q",
			"R", "S", "T", "U", "V", "W", "X", "Y", "Z",

			"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",

			"Esc",
			"Ctrl Izq.", "May. Izq.", "Alt Izq.", "Sistema Izq.",
			"Ctrl Der.", "May. Der.", "Alt Der.", "Sistema Der.",

			"Menu", "Abre cor.", "Cierra cor.", "Punto y coma", "Coma", "Punto",
			"Comillas", "Barra", "Barra inv.", "Tilde", "Igual", "Guion",

			"Barra espaciadora", "Enter", "Backspace", "Tab", "Page Up", "Page Down",
			"Fin", "Inicio", "Insertar", "Suprimir",
			"Agregar", "Restar", "Multiplicar", "Dividir",

			"Flecha Izq.", "Flecha Der.", "Flecha Arriba", "Flecha Abajo",
			"0 Numpad", "1 Numpad", "2 Numpad", "3 Numpad", "4 Numpad",
			"5 Numpad", "6 Numpad", "7 Numpad", "8 Numpad", "9 Numpad",

			"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
			"F11", "F12", "F13", "F14", "F15", "Pausa"
		};
		
		return keyNames[key];
	}


public:
	UserKeyPress() {
		// TODO Get keys from file
		player1.goUp = sf::Keyboard::W;
		player1.goDown = sf::Keyboard::S;
		player1.goRight = sf::Keyboard::D;
		player1.goLeft = sf::Keyboard::A;
		player1.UseBomb = sf::Keyboard::Space;
		player1.MakeAction = sf::Keyboard::E;

		player2.goUp = sf::Keyboard::Up;
		player2.goDown = sf::Keyboard::Down;
		player2.goLeft = sf::Keyboard::Left;
		player2.goRight = sf::Keyboard::Right;
		player2.UseBomb = sf::Keyboard::RControl;
		player2.MakeAction = sf::Keyboard::RShift;
	}

	PlayerEntity::PlayerControls &getPlayerControls(int playerNum) {
		switch (playerNum) {
		case 1:
			return player1;
		case 2: 
			return player2;
		}
		cerr << "getPlayerControls number out of bounds (must be 1 or 2)" << endl;
		return player1;
	}

	//bool TestEvent(sf::Keyboard::Key k, sf::Event e) {
	//	// Keyboard event
	//	if (k == e.key.code) {
	//		return (true);
	//	}
	//	return (false);
	//}

//	void userActions(sf::Event& event, sf::RenderWindow*& window, GameDisplayController& gameDisplay, Game& game) {
//		// Ignore key events
//
//		/*if (event.type == sf::Event::KeyPressed) {
//			cout << getKeyName(event.key.code) << " - " << event.key.code << endl;
//
//			if (TestEvent(Keys["P1 UP"], event)) {
//				std::cout << "P1 UP" << std::endl;
//			}
//			else if (TestEvent(Keys["P1 DOWN"], event)) {
//				std::cout << "P1 DOWN" << std::endl;
//			}
//			else if (TestEvent(Keys["P1 LEFT"], event)) {
//				std::cout << "P1 LEFT" << std::endl;
//			}
//			else if (TestEvent(Keys["P1 RIGHT"], event)) {
//				std::cout << "P1 RIGHT" << std::endl;
//			}
//			else if (TestEvent(Keys["P1 BOMB"], event)) {
//				std::cout << "P1 BOMB" << std::endl;
//			}
//			else if (TestEvent(Keys["P1 ACTION"], event)) {
//				std::cout << "P1 ACTION" << std::endl;
//			}
//
//			else if (TestEvent(Keys["P2 UP"], event)) {
//				std::cout << "P2 UP" << std::endl;
//			}
//			else if (TestEvent(Keys["P2 DOWN"], event)) {
//				std::cout << "P2 DOWN" << std::endl;
//			}
//			else if (TestEvent(Keys["P2 LEFT"], event)) {
//				std::cout << "P2 LEFT" << std::endl;
//			}
//			else if (TestEvent(Keys["P2 RIGHT"], event)) {
//				std::cout << "P2 RIGHT" << std::endl;
//			}
//			else if (TestEvent(Keys["P2 BOMB"], event)) {
//				std::cout << "P2 BOMB" << std::endl;
//			}
//			else if (TestEvent(Keys["P2 ACTION"], event)) {
//				std::cout << "P2 ACTION" << std::endl;
//			}
//		}*/
//	}
//
//
//
//public:


	/*void checkUserKeysPress(GameDisplayController& gameDisplay, Game& game){
		gameDisplay.manageGameInterface(gameDisplay, std::bind(&UserKeyPress::userActions, this, std::placeholders::_1, std::ref(gameDisplay.getWindow()), std::ref(gameDisplay), std::ref(game)));
	}*/
};