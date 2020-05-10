#pragma once

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "../Exceptions/ExceptionsGame.hpp"

#include "Estados.hpp"


using namespace std;

class StateGenerator
{

    bool canActivatePatrol = false;
    bool canActivateKill = false;
    bool canActivateSeekPE = false;
    int RangeVision = 0;

    struct PatrolStruct
    {
    public:
        float bootsPatrol = 0;
        sf::Vector2i puntRelativoA;
        sf::Vector2i puntRelativoB;
    };

    struct PerseguirStruct{
        int RangoVision = 0;
        int costDestroyWall = 0;
    };

    struct KillStruct
    {
    public:
        float bootsPatrol = 0;
        sf::Vector2i puntRelativoA;
        sf::Vector2i puntRelativoB;
    };

    struct SeekPEStruct
    {
    public:
        float bootsPatrol = 0;
        sf::Vector2i puntRelativoA;
        sf::Vector2i puntRelativoB;
    };

public:
    PatrolStruct _PatrolStruct;
    bool havePatrolStruct = false;
    KillStruct _KillStruct;
    bool haveKillStruct = false;
    SeekPEStruct _SeekPEStruct;
    bool haveSeekPEStruct = false;
    PerseguirStruct _PerseguirStruct;
    bool havePerseguirStruct = false;

    PatrolState getPatrol(sf::Vector2i pos);
    void generateState(ifstream &f, string estado);
    void readIA(string path);
};
