#pragma once

/*
Key binding tutorial from SFML https://github.com/SFML/SFML/wiki/Tutorial:-Manage-dynamic-key-binding
 */
class UserKeyPress{
	std::map<std::string, sf::Keyboard::Key> Keys;
	bool EsqPressed = false;

	PlayerEntity::PlayerControls player1;
	PlayerEntity::PlayerControls player2;

public:
	/*
	Get human redeable key name
	*/
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

	UserKeyPress(PlayerEntity::PlayerControls p1, PlayerEntity::PlayerControls p2) {
		player1 = p1;
		player2 = p2;
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
};