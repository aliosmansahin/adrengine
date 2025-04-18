#include "Graphics.h"

Graphics::~Graphics()
{
    if (shaderManager) delete shaderManager;
    Logger::Log("P", "Cleared graphics");
}

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
    shaderManager = new ShaderManager();
    if (!shaderManager->InitShaderManager())
        return false;
    if (!shaderManager->InitShaders(SHADER_2D))
        return false;

    //window size callback
    glfwSetWindowSizeCallback(window, StaticWindowSizeCallback);
    return true;
}

void Graphics::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Graphics::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
}
