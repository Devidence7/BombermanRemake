#pragma once
#include <iostream>
#include <chrono>
#include <ctime>  

class GameTime {
private:
	static std::chrono::time_point<std::chrono::system_clock> start;

public:
	// Set starting time and global GTime variable:
	GameTime()
	{
		GameTime::start = std::chrono::system_clock::now();
	}
	
	// Get current time in seconds:
	static double getTimeNow()
	{
		// Some computation here
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		//std::cout <<  "elapsed time: " << elapsed_seconds.count() << "s\n";
		return elapsed_seconds.count();
	}
};

std::chrono::time_point<std::chrono::system_clock> GameTime::start;
