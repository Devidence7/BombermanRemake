#include "TextureStorage.h"

PlayerTexture TextureStorage::playerTexture;
PlayerColor TextureStorage::playerColor;
WallTexture TextureStorage::level1WallTexture;
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

WallTexture &TextureStorage::getlevel1WallTexture()
{
	return level1WallTexture;
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

