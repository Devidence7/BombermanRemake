#include "../Map/Level.hpp"

PlayerIAEntity::PlayerIAEntity(PlayerControls &playerControls) : PlayerEntity(playerControls){}

//	sf::FloatRect getGlobalBounds() const override; Modificar??

//	void update() override;
void PlayerIAEntity::setCollision(std::shared_ptr<Entity> col){
	this->Entity::setCollision(col);
	if(col != nullptr){
		OmittedAreas.push_back(OmittedArea(getMapCoordinates(col->getCenterPosition())));
	}
}

//	void realizeActions();

bool PlayerIAEntity::playerActions(){

}
/*
	 * Update player position.
	 */
bool PlayerIAEntity::updatePlayer(){

	//TODO: Implementar planificador
	

}