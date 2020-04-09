#pragma once
#include <iostream>
#include <chrono>
#include <ctime>  

class GameTime {
private:
	static std::chrono::time_point<std::chrono::system_clock> start;

public:
	// Set starting time and global GTime variable:
	GameTime();
	
	// Get current time in seconds:
	static double getTimeNow();
};

