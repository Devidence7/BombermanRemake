#include "Time.h"

std::chrono::time_point<std::chrono::system_clock> GameTime::start;

GameTime::GameTime()
{
	GameTime::start = std::chrono::system_clock::now();
}

// Get current time in seconds:
double GameTime::getTimeNow()
{
	// Some computation here
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	//std::cout <<  "elapsed time: " << elapsed_seconds.count() << "s\n";
	return elapsed_seconds.count();
}

