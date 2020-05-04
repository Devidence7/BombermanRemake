#include "Time.h"

std::chrono::time_point<std::chrono::system_clock> GameTime::start;
double GameTime::lastTimeRecorded;

std::chrono::time_point<std::chrono::system_clock> GameTime::startPause;
double GameTime::timeInPause;

void GameTime::startGameTime() {
	GameTime::start = std::chrono::system_clock::now();
	GameTime::updateCurrentTime();

	startPause = std::chrono::system_clock::now();
	timeInPause = 0;
}

void GameTime::updateCurrentTime() {
	// Some computation here
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
//	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	lastTimeRecorded = elapsed_seconds.count() - timeInPause;
}

// Get current time in seconds:
double GameTime::getTimeNow()
{
	return lastTimeRecorded;
}


void GameTime::stopGameTime() {
	startPause = std::chrono::system_clock::now();
}

void GameTime::resumeGameTime() {
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - startPause;
// 	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	timeInPause += elapsed_seconds.count();
}

