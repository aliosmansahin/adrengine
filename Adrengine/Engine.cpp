#include "Engine.h"


Engine::~Engine()
{
    Logger::Log("P", "Clearing engine");
}

bool Engine::InitEngine(GLFWwindow* window)
{
    Logger::Log("P", "Starting engine");
    //set class variables
    this->window = window;

    //graphics engine
    graphics = new Graphics();
    if (!graphics->InitGraphics(window))
        return false;

    return true;
}

void Engine::Update()
{
}

void Engine::Draw()
{
    /* Render here */
    graphics->Clear();
}
