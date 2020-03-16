#pragma once


#define sizeTextureX 48
#define sizeTextureY 48

#define SIZE_PILLAR 48


enum LookingAt {
		down = 0, up = 1, left = 2, right = 3
	};

enum LookingAtBi {
	bRight = 1, bLeft = 0
};

enum PlayerEvent{
    MOVE = 0, PUT_BOMB, STOPED
};

enum EnemyType {
	balloon = 0,
	ice = 1,
	barrel = 4,
	coin = 2,
	blob = 6,
	ghost = 7,
	hypo = 8
	
	
};

const int TOTAL_ENEMY_FRAMES = 13;


// Debug features:
bool HITBOX_DEBUG_MODE = true;