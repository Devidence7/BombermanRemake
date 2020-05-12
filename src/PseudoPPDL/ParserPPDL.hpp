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
public:
    bool canActivatePatrol = false;
    bool canActivateKill = false;
    bool canActivateSeekPE = false;
    int RangeVision = 0;
    float intersetBuffPU = 0;
    float intersetNerfPU = 0;
    float intersetACtionPU = 0;
    float bonusDestroyWalls = 0;
    float imprudencia = 0;


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
        float imprudencia = 0;
        int costDestroyWall = 0;
        int ansiansDeKill = 0;
    };

    struct SeekPEStruct
    {
    public:

        float bootsSeek = 0;
        int RangoVision = 0;
        float interBuffPU = 0;
        float interNerfPU = 0;
        float interActionPU = 0;

    };

    PatrolStruct _PatrolStruct;
    bool havePatrolStruct = false;
    KillStruct _KillStruct;
    bool haveKillStruct = false;
    SeekPEStruct _SeekPEStruct;
    bool haveSeekPEStruct = false;
    PerseguirStruct _PerseguirStruct;
    bool havePerseguirStruct = false;

    PatrolState getPatrol(sf::Vector2i pos);
    PerseguirState getVisionPerseguir();
    PerseguirState getCostDestWallPerseguir();
    KillState getImprudencia();
    KillState getVisionKill();
    SeekPEState getVisionSeek();
    SeekPEState getInterBuffPU();
    SeekPEState getInterActionPU();
    



    
   // PerseguirState get
    void generateState(ifstream &f, string estado);
    void readIA(string path);
};
