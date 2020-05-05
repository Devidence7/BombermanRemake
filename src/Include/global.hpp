#pragma once

#define sizeTextureX 48
#define sizeTextureY 48

#define SIZE_PILLAR 48
#define SIZE_PILLAR_2 SIZE_PILLAR/2

#define SHOOTING_DISTANCE 5

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
	moreSpeed = 1,
	moreFire = 2,
	lessFire = 3,
	moreBombs = 4,
	kickBombs = 6,
	grabBombs = 7,
	extraLife = 14,
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

const int TOTAL_ENEMY_FRAMES = 13;
const int THRESHOLD_CENTER_CELL = 2;
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

inline bool checkArrivePosition(const sf::Vector2f &currentPos, const sf::Vector2i &objetive, const sf::Vector2i &direction){
	sf::Vector2f objetiveGlobalCoord = MapCoordinates2GlobalCoorCenter(objetive);
	//cruzar el objetivo
	objetiveGlobalCoord.x += direction.x;
	objetiveGlobalCoord.y += direction.y;

	return abs(objetiveGlobalCoord.x - currentPos.x) < THRESHOLD_CENTER_CELL && abs(objetiveGlobalCoord.y - currentPos.y) < THRESHOLD_CENTER_CELL;
}