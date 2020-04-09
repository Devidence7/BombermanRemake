#include "TextureStorage.h"

PlayerTexture TextureStorage::playerTexture;
WallTexture TextureStorage::level1WallTexture;
BombTexture TextureStorage::bombTexture;
FireTexture TextureStorage::fireTexture;
EnemyTexture TextureStorage::enemyTexture;
PowerUpTexture TextureStorage::powerUpTexture;


TextureStorage::TextureStorage() {}

PlayerTexture &TextureStorage::getPlayerTexture()
{
	return playerTexture;
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

