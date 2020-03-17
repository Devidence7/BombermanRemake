#pragma once
#include <random>

class Random {
	static Random randomGen;

	std::mt19937 mt;

	Random() : mt(std::random_device()()) {
	}

public:
	static void initilizeRandomGen() {
		randomGen = Random();
	}

	static int getIntNumberBetween(int a, int b) {
		std::uniform_int_distribution<int> dist(a, b);
		return dist(randomGen.mt);
	}
};

Random Random::randomGen;