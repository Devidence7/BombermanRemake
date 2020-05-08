#include "EnemyTexture.h"

EnemyTexture::EnemyTexture() {
	frameSize.x = 49;
	frameSize.y = 49;

	// Get texture sprites:
	if (!this->texture.loadFromFile("../textures/Entities/enemies.png"))
	{
		std::cerr << "Unable to load texture:" << std::endl;
		std::cerr << "Texture location: ../textures/Entities/enemies.png" << std::endl;
	}

	// Save sprites positions in a array
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			sf::IntRect frame((frameSize.x + 6) * x, (frameSize.y + 6) * y, frameSize.x, frameSize.y);
			frames.push_back(frame);
		}
	}
}

sf::IntRect &EnemyTexture::getIdleSprite(int lookat, EnemyType enemyType)
{
	return frames[TOTAL_ENEMY_FRAMES * enemyType + lookat * 4];
}

sf::IntRect &EnemyTexture::getWalkingSprite(LookingAtBi looking, int i, EnemyType enemyType)
{
	return frames[TOTAL_ENEMY_FRAMES * enemyType + 4 * looking + i];
}

sf::IntRect &EnemyTexture::getDeathSprite(int i, EnemyType enemyType)
{
	return frames[TOTAL_ENEMY_FRAMES * enemyType + 8 + i];
}
