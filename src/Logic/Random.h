#pragma once
#include <random>

class Random {
	static Random randomGen;

	std::mt19937 mt;

	Random();
public:
	static void initilizeRandomGen();

	static int getIntNumberBetween(int a, int b);
	static float getFloatNumberBetween(float a, float b);
};

//Random Random::randomGen;

