#pragma once


#define sizeTextureX 48
#define sizeTextureY 48


enum LookingAt {
		down = 0, up = 1, left = 2, right = 3
	};

enum LookingAtBi {
	bRight = 0, bLeft = 1
};

enum PlayerEvent{
    MOVE = 0, PUT_BOMB, STOPED
};

enum EnemyType {
	balloon = 0,
	ice = 1
};

const int TOTAL_ENEMY_FRAMES = 13;