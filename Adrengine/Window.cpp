#include "Window.h"

Window::~Window()
{
    CloseWindow();
}

bool Window::CreateWindow(int width, int height, const char* title)
{
    /* Initialize the library */
    Logger::Log("P", "Creating window");

    if (!glfwInit()) {
        Logger::Log("E", "GLFW Initialization failed in glfwInit");
        return false;
    }

    isGLFWInited = true;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        Logger::Log("E", "Creating window failed in glfwCreateWindow");
        return false;
    }

    return true;
}

void Window::CloseWindow()
{
    Logger::Log("P", "Closing window");
    if (isGLFWInited) {
        glfwTerminate();
    }
}
