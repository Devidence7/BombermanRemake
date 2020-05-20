#include "TextureStorage.h"

PlayerTexture TextureStorage::playerTexture;
PlayerColor TextureStorage::playerColor;
EntityShadowTexture TextureStorage::entityShadowTexture;
WallTexture TextureStorage::level1WallTexture;
WallShadowTexture TextureStorage::wallShadowTexture;
BombTexture TextureStorage::bombTexture;
FireTexture TextureStorage::fireTexture;
EnemyTexture TextureStorage::enemyTexture;
PowerUpTexture TextureStorage::powerUpTexture;
TeleporterTexture TextureStorage::teleporterTexture;



TextureStorage::TextureStorage() {}

PlayerTexture &TextureStorage::getPlayerTexture()
{
	return playerTexture;
}

PlayerColor &TextureStorage::getPlayerColor() {
	return playerColor;
}

EntityShadowTexture& TextureStorage::getEntityShadowTexture() {
	return entityShadowTexture;
}

WallTexture &TextureStorage::getlevel1WallTexture()
{
	return level1WallTexture;
}

WallShadowTexture& TextureStorage::getWallShadowTexture() {
	return wallShadowTexture;
}

BombTexture &TextureStorage::getBombTexture()
{
	return bombTexture;
}

FireTexture &TextureStorage::getFireTexture()
{
	return fireTexture;
}

EnemyTexture &TextureStorage::getEnemyTexture()
{
	return enemyTexture;
}

PowerUpTexture &TextureStorage::getPowerUpTexture()
{
	return powerUpTexture;
}

TeleporterTexture &TextureStorage::getTeleporterTexture()
{
	return teleporterTexture;
}

