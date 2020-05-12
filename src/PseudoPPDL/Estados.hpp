#pragma once
#include <SFML/Graphics.hpp>
#include "../Include/global.hpp"
#include "../GameMaths/GeometryMaths.hpp"
inline bool isOn(sf::Vector2i IA, sf::Vector2i o){
    return IA.x == o.x && IA.y == o.y;
}

inline bool isOn(sf::Vector2f IA, sf::Vector2f o){
    return (moduleVector(IA - o) < 3);
}


class PatrolState{
    public:
    sf::Vector2i objetivo1;
    sf::Vector2i objetivo2;

    PatrolState(){}
    PatrolState(sf::Vector2i o1, sf::Vector2i o2): objetivo1(o1), objetivo2(o2){}

    sf::Vector2i getObetive(sf::Vector2i IA){
        if(isOn(IA, objetivo1)){return objetivo2;}
        return objetivo1;
    }

};

class PerseguirState{
    public:
      int rangeVision ;
      int costDestWall;

    PerseguirState(){}
    PerseguirState(int rangoVision,int costDestroyWall): rangeVision(rangoVision),costDestWall(costDestroyWall){}

    int getVisionPerseguir(){
        return rangeVision;
    }

    int getCostDestWallPerseguir(){
        return costDestWall;
    }

};

class KillState{
    public:
      float imprudencia ;
      int costDestWall ;
      float agresivity;

    KillState(){}
    KillState(float imp,int costDestroyWall): imprudencia(imp),costDestWall(costDestroyWall){}

    float getImprudencia(){
        return imprudencia;
    }

    int getCostDestWallKill(){
        return costDestWall;
    }

};


class SeekPEState{
    public:
        int RangoVision ;
        float interBuffPU ;
        float interNerfPU ;
        float interActionPU ;

    SeekPEState(){}
    SeekPEState(int rangeVision,float buff,float nerf,float action): RangoVision(rangeVision),interBuffPU(buff),interNerfPU(nerf),interActionPU(action){}

     int getVisionSeek(){
        return RangoVision;
    }

    float getInterBuffPU(){
        return interBuffPU;
    }

    float getInterNerfPU(){
        return interNerfPU;
    }
    
    float getInterActionPU(){
        return interActionPU;
    }
    
};




