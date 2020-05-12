#include "ParserPPDL.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

//Parser
void StateGenerator::generateState(ifstream &f, string estado)
{
    std::size_t found = estado.find("patrullar");
    if (found != std::string::npos)
    {
        sf::Vector2i x, y;
        f >> x.x >> x.y >> y.x >> y.y;
        this->_PatrolStruct.puntRelativoA = x;
        this->_PatrolStruct.puntRelativoB = y;
        this->havePatrolStruct = true;
        return;
    }
    found = estado.find("perseguir");
    if (found != std::string::npos){
        this->havePerseguirStruct = true;
        for(int i = 0; i < 2; i++){
            string text;
            std::getline(f, text);
            // boost::algorithm::to_lower(text);
            std::transform(text.begin(), text.end(), text.begin(),
                [](unsigned char c) { return std::tolower(c); });
            found = text.find("rangovision");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("rangovision"), text.size()) ;
                std::cout <<"rangovision "<< val << std::endl;
                this->_PerseguirStruct.RangoVision = stoi(val);
            }
            found = text.find("costdestroywall");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("costdestroywall"), text.size()) ;
                std::cout <<"costdestroywall "<< val << std::endl;
//                this->_PerseguirStruct.costDestroyWall = val;           
                this->_PerseguirStruct.costDestroyWall = stoi(val);
            }
        }
    }
     found = estado.find("matar");
      if (found != std::string::npos){
        for(int i = 0; i < 3; i++){
            string text;
            std::getline(f, text);
            // boost::algorithm::to_lower(text);
            std::transform(text.begin(), text.end(), text.begin(),
                [](unsigned char c) { return std::tolower(c); });
            found = text.find("imprudencia");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("rangovision"), text.size()) ;
                std::cout <<"matar"<< val << std::endl;
                this->_KillStruct.imprudencia = stoi(val);
            }
            found = text.find("costdestroywall");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("costdestroywall"), text.size()) ;
                std::cout << "matar "<<val << std::endl; 
//                this->_PerseguirStruct.costDestroyWall = val;           
                this->_KillStruct.costDestroyWall = stoi(val);
                bonusDestroyWalls=stoi(val);
            }

            found = text.find("agresivity");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("agresivity"), text.size()) ;
                std::cout << "matar "<<val << std::endl; 
//                this->_PerseguirStruct.costDestroyWall = val;           
                this->_KillStruct.ansiansDeKill = stoi(val);
            }
        }
      }

      found = estado.find("buscarpu");
      if (found != std::string::npos){
          for(int i = 0; i < 2; i++){
            string text;
            std::getline(f, text);
            // boost::algorithm::to_lower(text);
            std::transform(text.begin(), text.end(), text.begin(),
                [](unsigned char c) { return std::tolower(c); });
                cout<<"TEXT: "<<text<<endl;
            found = text.find("intergen");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("intergen"), text.size()) ;
                std::cout << "intergen"<<val << std::endl;
                this->_SeekPEStruct.bootsSeek = stoi(val);
            }

            found = text.find("rangovision");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("rangovision"), text.size()) ;
                std::cout << "rangovision "<<val << std::endl;
                this->_SeekPEStruct.RangoVision = stoi(val);
            }
          }

            float buff, nerf,action;
            f>>buff>>nerf>>action;
           
             this->_SeekPEStruct.interBuffPU = buff;
             this->_SeekPEStruct.interNerfPU = nerf;
             this->_SeekPEStruct.interActionPU =action;
             this->haveSeekPEStruct = true;
             return;
          

      }
    
}

void StateGenerator::readIA(string path)
{
    ifstream fileIA(path);
    if (!fileIA.is_open())
    {
        cerr << "La IA " + path + " no existe" << endl;
        throw FileIADoesntExist("La IA " + path + " no existe");
    }
    string line;
    std::getline(fileIA, line);
    try
    {
        while (!fileIA.eof())
        {
            //boost::algorithm::to_lower(line);
            std::transform(line.begin(), line.end(), line.begin(),
                [](unsigned char c) { return std::tolower(c); });
            std::size_t found = line.find("estado");
            if (found != std::string::npos)
            {
                string estado = line.substr(found, line.size());
                generateState(fileIA, estado);
            }
            std::getline(fileIA, line);
        }
    }
    catch (exception)
    {
        fileIA.close();
        throw BadReadFileIA("Error al parsear fichero " + path);
    }

    fileIA.close();
}


//Generacion de estados
PatrolState StateGenerator::getPatrol(sf::Vector2i pos)
{
    return PatrolState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}
/*
PerseguirState StateGenerator::getVisionPerseguir()
{
    return _PerseguirStruct.RangoVision;
}

PerseguirState StateGenerator::getCostDestWallPerseguir()
{
    return PerseguirState(pos + _PerseguirStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}

KillState StateGenerator::getImprudencia()
{
    return KillState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}

KilllState StateGenerator::getVisionKill()
{
    return KillState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}
SeekPEState StateGenerator::getVisionSeek()()
{
    return SeekPEState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}

SeekPEState StateGenerator::getInterBuffPU()
{
    return SeekPEState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}
SeekPEState StateGenerator::getInterNerfPU()
{
    return PatrolState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}
SeekPEState StateGenerator::getInterActionPU()
{
    return PatrolState(pos + _PatrolStruct.puntRelativoA, pos + _PatrolStruct.puntRelativoB);
}
*/
