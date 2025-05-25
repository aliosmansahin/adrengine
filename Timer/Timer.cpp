#include "pch.h"
#include "Timer.h"

//Implementations for static variables
float Timer::deltaTime;
std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> Timer::chronos;

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

TIMER_API void Timer::StartChrono(std::string id)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    chronos[id] = startTime;
}

TIMER_API float Timer::EndChrono(std::string id)
{
    auto iter = chronos.find(id);
    if (iter == chronos.end())
        return 0.0f;

    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = endTime - iter->second;

    chronos.erase(iter);

    return (float)duration.count();
}
