#pragma once
#include <SFML/Graphics.hpp>

#define sizeTextureX 48
#define sizeTextureY 48

#define SIZE_PILLAR 48
#define SIZE_PILLAR_2 SIZE_PILLAR/2

#define SHOOTING_DISTANCE 5

enum ActionsAvalible {
	NONE_ACTION = 0,
	GRAB_BOMB = 7,
	THROUGH_BOMB = 12,
	REMOTE_BOMB = 5,
	KICK_BOM = 6
};

struct GameOptions {
	int numPlayers = 0;
	double difLevel = 1;
	int numIAPlayers = 0;
	bool historyMode = false;
	int playersAndTeams[4] = {0,0,0,0};
	int numTeams = 0;
	sf::Color* colorList;
	int player1ColorPick = 0;
	int player2ColorPick = 1;

	// Multi options
	int selectedStage = 1;
	int selectedSizeStage = 1;
	int IADifficulty = 1;
	ActionsAvalible startingAbility = ActionsAvalible::NONE_ACTION;
	int percentageOfWalls = 66;
	int startingLives = 3;
};

class GlobalTime{
public:
static double totalTime;
static double timeLeft;
};

enum LookingAt
{
	down = 0,
	up = 1,
	left = 2,
	right = 3
};

enum LookingAtBi
{
	bRight = 1,
	bLeft = 0
};

enum PlayerEvent
{
	MOVE = 0,
	PUT_BOMB,
	STOPED
};



enum EnemyType
{
	balloon = 0,
	ice = 1,
	barrel = 4,
	coin = 2,
	blob = 6,
	ghost = 7,
	hypo = 8
};

enum PowerUpType
{
	dissease = 0,
	moreSpeed = 1,
	moreFire = 2,
	lessFire = 3,
	moreBombs = 4,
	remoteBombs = 5,
	kickBombs = 6,
	grabBombs = 7,
	passBombs = 12,
	extraLife = 17,
	moreTime = 18,
	lessSpeed = 19
};

enum CollisionType
{
	NONE = 0,
	HORIZONTAL = 1,
	VERTICAL = 2,
	CORNER = 3,
};

enum TypePositionRelative{
	OBJETIVE_POSITION = 0,
	NEAR_POSITION = 1,	//Tratar de atajar al jugador yendo a la posicón cercana
	LOKIN_POSITION = 2 //Tratar de ir dónde está mirando el jugador

};

enum TypeSeekIA
{
	BEST_PATH = 0,
	SECOND_BEST_PATH = 1, //Tratar de llegar por un camino no tan directo
	LONG_PATH = 2, //Tratará de llegar a cualquier posicón dónde pueda ir el jugador
};

const int TOTAL_ENEMY_FRAMES = 13;
const int THRESHOLD_CENTER_CELL = 5;
// Debug features:
#define HITBOX_DEBUG_MODE true;

////////////////////////////////////////////
///////// MAP COORDENATE OPERATOR /////////
///////////////////////////////////////////

inline sf::Vector2i getMapCoordinates(float x, float y)
{
	return sf::Vector2i((int)x / SIZE_PILLAR, (int)y / SIZE_PILLAR);
}

inline  sf::Vector2i getMapCoordinates(sf::Vector2f pos)
{
	return getMapCoordinates(pos.x, pos.y);
}


inline sf::Vector2f MapCoordinates2GlobalCoorCorner(const int x, const int y)
{
	return sf::Vector2f((float)x * SIZE_PILLAR, (float)y * SIZE_PILLAR);
}

inline sf::Vector2f MapCoordinates2GlobalCoorCorner(sf::Vector2i pos)
{
	return MapCoordinates2GlobalCoorCorner(pos.x, pos.y);
}


inline sf::Vector2f MapCoordinates2GlobalCoorCenter(const int x, const int y)
{
	return sf::Vector2f((float)x * SIZE_PILLAR + SIZE_PILLAR/2, (float)y * SIZE_PILLAR + SIZE_PILLAR/2);
}

inline  sf::Vector2f MapCoordinates2GlobalCoorCenter(const sf::Vector2i pos)
{
	return MapCoordinates2GlobalCoorCenter(pos.x, pos.y);
}

inline bool checkIsOnCenterCell(const sf::Vector2f &currentPos, const sf::Vector2i &objetive){
	sf::Vector2f objetiveGlobalCoord = MapCoordinates2GlobalCoorCenter(objetive);
	//cruzar el objetivo
	return abs(objetiveGlobalCoord.x - currentPos.x) < THRESHOLD_CENTER_CELL && abs(objetiveGlobalCoord.y - currentPos.y) < THRESHOLD_CENTER_CELL;
}

inline bool checkArrivePosition(const sf::Vector2f &currentPos, const sf::Vector2i &objetive, const sf::Vector2i &direction){
	sf::Vector2f objetiveGlobalCoord = MapCoordinates2GlobalCoorCenter(objetive);
	//cruzar el objetivo
	objetiveGlobalCoord.x += direction.x;
	objetiveGlobalCoord.y += direction.y;

	return abs(objetiveGlobalCoord.x - currentPos.x) < THRESHOLD_CENTER_CELL && abs(objetiveGlobalCoord.y - currentPos.y) < THRESHOLD_CENTER_CELL;
}