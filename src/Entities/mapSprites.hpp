#include "Entity.h"
#define sizeColliderPillar 48

class Pillar : public Entity{

    public:
    Pillar(int x, int y) : Entity(new Collider2d(sf::Vector2f(x,y), sf::FloatRect(0,0,sizeColliderPillar,sizeColliderPillar))){}
    void update(){}

};