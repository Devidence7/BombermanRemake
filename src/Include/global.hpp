#pragma once

#define sizeTextureX 48
#define sizeTextureY 48

#define SIZE_PILLAR 48

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
	moreBombs = 4
};

enum CollisionType
{
	NONE = 0,
	HORIZONTAL = 1,
	VERTICAL = 2,
	CORNER = 3,
};

const int TOTAL_ENEMY_FRAMES = 13;

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


inline sf::Vector2f MapCoordinates2GlobalCoorCenter(const int x, const int y)
{
	return sf::Vector2f((float)x * SIZE_PILLAR + SIZE_PILLAR/2, (float)y * SIZE_PILLAR + SIZE_PILLAR/2);
}

inline  sf::Vector2f MapCoordinates2GlobalCoorCenter(const sf::Vector2i pos)
{
	return MapCoordinates2GlobalCoorCenter(pos.x, pos.y);
}

