#include "Timer.h"

float Timer::deltaTime;

void Timer::Update()
{
    float currentTime = (float)glfwGetTime();
    static float lastTime = currentTime; // for the first frame, set the last time to current time

    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

float Timer::GetDeltaTime()
{
    return deltaTime;
}
