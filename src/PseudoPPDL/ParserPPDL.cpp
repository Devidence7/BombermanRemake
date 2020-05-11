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
                std::cout << val << std::endl;
                this->_PerseguirStruct.RangoVision = stoi(val);
            }
            found = text.find("costdestroywall");
            if (found != std::string::npos){
                string val = text.substr(found + sizeof("costdestroywall"), text.size()) ;
                std::cout << val << std::endl;
//                this->_PerseguirStruct.costDestroyWall = val;           
                this->_PerseguirStruct.costDestroyWall = stoi(val);
            }
        }
    }
    
}

void StateGenerator::readIA(string path)
{
    ifstream fileIA(path);
    if (!fileIA.is_open())
    {
        //throw FileIADoesntExist("La IA " + path + " no existe");
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
