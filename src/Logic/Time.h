#pragma once
#include <iostream>
#include <chrono>
#include <ctime>  

class GameTime {
private:
	static std::chrono::time_point<std::chrono::system_clock> start;
	static double lastTimeRecorded;
	
	static std::chrono::time_point<std::chrono::system_clock> startPause;
	static double timeInPause;

public:
	// Set starting time and global GTime variable:
	static void startGameTime();

	// Updates the current time for not making calculations all the time.
	static void updateCurrentTime();
	
	// Get current time in seconds:
	static double getTimeNow();

	static void stopGameTime();

	static void resumeGameTime();
};

