#include "Teleporter.h"
#include "../Logic/Time.h"

Teleporter::Teleporter(sf::Vector2f pos) : Entity() {
	isFireDestroyable = false;
	fireCanGoThroght = false;
	this->collisioner = false;

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) {
		openTeleporter();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) {
		teleporterState = TeleporterState::CLOSE;
	}
}
//
void Teleporter::onCollission(std::shared_ptr<Entity> eCollisioning, std::shared_ptr<Entity> eCollisioner, CollisionType colT) {
//	std::shared_ptr<PlayerEntity> pe;
//	if (!this->expiredEntity && (pe = std::dynamic_pointer_cast<PlayerEntity>(eCollisioning)) != nullptr) {
//		// TODO: NEXT STAGE
//	}
}

void Teleporter::openTeleporter() {
	teleporterState = TeleporterState::OPEN;
}