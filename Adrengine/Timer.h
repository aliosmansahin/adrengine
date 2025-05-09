#pragma once

#include <GLFW/glfw3.h>

class Timer
{
public:
	static void Update();
	static float GetDeltaTime();
private:
	Timer() = default;
	~Timer() = default;
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
private:
	static float deltaTime;

};