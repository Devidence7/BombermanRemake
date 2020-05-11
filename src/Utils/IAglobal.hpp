
#pragma once

enum StateIA{
    NON_OBJETIVE = 0,
    PATROL = 1,
    KILL = 2,
    CATCH_PU = 3,
    PERSEGUIR = 4,
    RUNAWAY = 5,
    FARM = 6
};


enum TypeAction{
    ACTION_MOVE_TO = 0,
    ACTION_DESTROY_WALLS = 1,
    ACTION_PUT_BOMB = 2
};