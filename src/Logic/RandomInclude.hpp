#include "Random.h"
#include "Time.h"

std::chrono::time_point<std::chrono::system_clock> GameTime::start;

Random Random::randomGen;
