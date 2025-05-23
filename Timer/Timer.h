#pragma once

#include <GLFW/glfw3.h>

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
private:
	//singleton
	Timer() = default;
	~Timer() = default;
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
private:
	//variables
	static float deltaTime;
};