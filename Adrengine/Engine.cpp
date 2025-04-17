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
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (version == 0) {
        Logger::Log("E", "Starting engine failed in gladLoadGLLoader");
        return false;
    }
    
    Logger::Log("P", "Loaded OpenGL Version 3.3");



    return true;
}

void Engine::Update()
{
}

void Engine::Draw()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
}
