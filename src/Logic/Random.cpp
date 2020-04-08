#include "Random.h"
Random Random::randomGen;
Random::Random() : mt(std::random_device()())
{
}

void Random::initilizeRandomGen()
{
	randomGen = Random();
}

int Random::getIntNumberBetween(int a, int b)
{
	std::uniform_int_distribution<int> dist(a, b);
	return dist(randomGen.mt);
}