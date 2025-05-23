#include "pch.h"
#include "Timer.h"

//Implementations for static variables
float Timer::deltaTime;

/*
PURPOSE: Updates timer stuff (currently updates only deltaTime)
*/
void Timer::Update()
{
    if (!glfwInit()) {
        Logger::Log("E", "GLFW initialization failed in DLL!");
    }
    float currentTime = (float)glfwGetTime();
    static float lastTime = currentTime; // for the first frame, set the last time to current time

    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

/*
PURPOSE: Return deltaTime
*/
float Timer::GetDeltaTime()
{
    return deltaTime;
}

/*
PURPOSE: Returns current time
*/
TIMER_API float Timer::GetCurTime()
{
    return (float)glfwGetTime();
}
