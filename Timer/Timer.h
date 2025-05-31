#pragma once

#include "glfw/glfw3.h"
#include <unordered_map>
#include <chrono>

#include "Logger.h"

#ifdef TIMER_EXPORTS
#define TIMER_API __declspec(dllexport)
#else
#define TIMER_API __declspec(dllimport)
#endif

class Timer
{
public:
	//main functions
	TIMER_API static void Update();

	//getters
	TIMER_API static float GetDeltaTime();
	TIMER_API static float GetCurTime();
	TIMER_API static void  StartChrono(std::string id);
	TIMER_API static float EndChrono(std::string id);
private:
	//singleton
	Timer() = default;
	~Timer() = default;
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
private:
	//variables
	static float deltaTime;
	static std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> chronos;
};