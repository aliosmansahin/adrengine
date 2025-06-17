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

/*
PURPOSE: Saves current time as a key
*/
TIMER_API void Timer::StartChrono(std::string id)
{
    //Get the current time and insert it into a unordered_map which is for storing all chronos
    auto startTime = std::chrono::high_resolution_clock::now();

    chronos[id] = startTime;
}

/*
PURPOSE: Calculates duration between current time and the time which is given by a key
*/
TIMER_API float Timer::EndChrono(std::string id)
{
    //Key the time given by key
    auto iter = chronos.find(id);
    if (iter == chronos.end())
        return 0.0f;

    //Get current time
    auto endTime = std::chrono::high_resolution_clock::now();

    //Calculate duration
    std::chrono::duration<double> duration = endTime - iter->second;

    //Delete key and value which is unnecessary after the calculation
    chronos.erase(iter);

    //Return calculated duration
    return (float)duration.count();
}
