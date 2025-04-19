#include "Graphics.h"

bool Graphics::InitGraphics(GLFWwindow* window)
{
    Logger::Log("P", "Initalizing graphics");
    this->window = window;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (version == 0) {
        Logger::Log("E", "Starting engine failed in gladLoadGLLoader");
        return false;
    }

    Logger::Log("P", "Loaded OpenGL Version 3.3");

    //init shader manager
    Logger::Log("P", "Initializing shader manager");

    if (!ShaderManager::GetInstance().InitShaderManager())
        return false;
    if (!ShaderManager::GetInstance().InitShaders(SHADER_2D))
        return false;
    

    int windowW, windowH;
    glfwGetFramebufferSize(window, &windowW, &windowH);
    ShaderManager::GetInstance().UpdateProjectionMatrix(windowW, windowH);

    //window size callback
    glfwSetWindowSizeCallback(window, StaticWindowSizeCallback);

    return true;
}

void Graphics::ReleaseGraphics()
{
    ShaderManager::GetInstance().ReleaseShaderManager();
    Logger::Log("P", "Cleared graphics");
}

Graphics& Graphics::GetInstance()
{
    static Graphics graphics;
    return graphics;
}

void Graphics::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    ShaderManager::GetInstance().UpdateProjectionMatrix(width, height);
}

void Graphics::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
}
