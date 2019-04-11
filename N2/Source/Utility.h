#ifndef UTILITY_H
#define UTILITY_H

#include <chrono>
#include <thread>
#include <iostream>

struct PerformanceClock {

	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	float* durationTime;

	PerformanceClock(float* durationTime) : durationTime(durationTime)
	{
		start = std::chrono::high_resolution_clock::now();
	}

	PerformanceClock()
	{
		durationTime = nullptr;
		start = std::chrono::high_resolution_clock::now();
	}

	~PerformanceClock()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		//std::cout << "Duration: " << duration.count() * 1000.0f << " ms" << std::endl;
		if (durationTime != nullptr)
			*durationTime = duration.count() * 1000.0f;
	}
};

#endif