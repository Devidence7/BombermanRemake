#include "IAFunctions.hpp"
#include "../Map/Level.hpp"
#include "../Include/EntitiesInclude.hpp"


sf::Vector2i generateObjetive2Player(Entity_ptr p, Entity_ptr e,  std::vector<sf::Vector2i> &objetives)
{
    sf::Vector2f posPlayer = p->getCenterPosition();
    sf::Vector2i posMapNeeded = getMapCoordinates(posPlayer);
    switch (e->typePosIA)
    {
    case TypePositionRelative::OBJETIVE_POSITION:
            objetives.push_back(posMapNeeded);
            std::cout << posMapNeeded.x << " " << posMapNeeded.y << "\n";
        break;
    case TypePositionRelative::NEAR_POSITION:
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (i != j && i != 0)
                    {
                        sf::Vector2i posObj = posMapNeeded + sf::Vector2i(i, j);
                        if(EntityMap::isValidCell(posObj)){
                            objetives.push_back(posObj);
                        }
                    }
                }
        }
        break;

    default:
        break;
    }
}



void seekAnyPlayerOrRandom(list<ANode_Ptr> & movements, Entity_ptr e)
{
    movements.clear();
    std::vector<sf::Vector2i> objetives;
    sf::Vector2i mapPosition = getMapCoordinates(e->getCenterPosition());
    if(e->rangoVision != 0){

        for(Player_ptr p: PLayers::getVectorPlayer()){
            //if is in range
            if(isOnVision(p, mapPosition, e->rangoVision)){
                generateObjetive2Player(p, e, objetives);
            }
        }
        if(objetives.size() > 0){
            generatePath(e, objetives, movements);
        }else{
        }
    }
	if(movements.size() < 1){
		generateRandomPath(mapPosition, movements, e);
	}
}