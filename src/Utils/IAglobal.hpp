
#pragma once
#include <string>

enum StateIA{
    NON_OBJETIVE = 0,
    PATROL = 1,
    KILL = 2,
    CATCH_PU = 3,
    PERSEGUIR = 4,
    RUNAWAY = 5,
    FARM = 6,
    THROWING_BOMB = 7
};

inline std::string State2string(StateIA ia ){
    switch (ia)
    {
    case StateIA::NON_OBJETIVE :
        return "NON_OBJETIVE";
    break;
    case StateIA::PATROL :
        return "PATROL";
    break;
    case StateIA::KILL :
        return "KILL";
    break;
    case StateIA::CATCH_PU :
        return "CATCH_PU";
    break;
    case StateIA::PERSEGUIR :
        return "PERSEGUIR";
    break;
    case StateIA::RUNAWAY :
        return "RUNAWAY";
    break;
    case StateIA::FARM :
        return "FARM";
    break;
    case StateIA::THROWING_BOMB:
        return "THROWING_BOMB";
    break;
    default:
        break;
    }
    return "";
}

enum TypeAction{
    ACTION_MOVE_TO = 0,
    ACTION_DESTROY_WALLS = 1,
    ACTION_PUT_BOMB = 2
};