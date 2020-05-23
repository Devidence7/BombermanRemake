#include "Teleporter.h"
#include "../Logic/Time.h"
#include "../Music/GameSounds.h"


//Teleporter::Teleporter() : Entity() {
//	isTeleport=true;
//	isFireDestroyable = false;
//	fireCanGoThroght = false;
//	collisioner = false;
//
//	spriteCounter = GameTime::getTimeNow();
//
//	// Texture Controller:
//	teleporterTexture = &TextureStorage::getTeleporterTexture();
//	// Set sprite Sheet texture
//	setTexture(teleporterTexture->getTexture());
//	// Set starting sprite
//	setTextureRect(teleporterTexture->getDefaultIntRect());
//
//	setPosition(100,100);
//}

Teleporter::Teleporter(sf::Vector2f pos) : Entity() {
	isTeleport=true;
	isFireDestroyable = false;
	fireCanGoThroght = false;
	collisioner = false;
	expiredEntity = false;

	spriteCounter = GameTime::getTimeNow();

	// Texture Controller:
	teleporterTexture = &TextureStorage::getTeleporterTexture();
	// Set sprite Sheet texture
	setTexture(teleporterTexture->getTexture());
	// Set starting sprite
	setTextureRect(teleporterTexture->getDefaultIntRect());

	setPosition(pos);
}

void Teleporter::update() {
	if (GameTime::getTimeNow() - spriteCounter > spriteSpeed) {
		currentFrame = (currentFrame + 1) % spriteFrames;
		setTextureRect(teleporterTexture->getFrame(currentFrame, teleporterState));
		spriteCounter = GameTime::getTimeNow();
	}
}


void Teleporter::onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT) {
	std::shared_ptr<PlayerEntity> pe;
	if (!this->expiredEntity && (pe = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr){
		if(teleporterState == OPEN){
			playerCross = true;
			GameSounds::teleportEnter();
		}
	}
}

void Teleporter::openTeleporter() {
	teleporterState = TeleporterState::OPEN;
}

void Teleporter::closeTeleporter() {
	teleporterState = TeleporterState::CLOSE;
}