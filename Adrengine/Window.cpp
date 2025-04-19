#include "Window.h"

bool Window::CreateWindow(int width, int height, const char* title)
{
    /* Initialize the library */
    Logger::Log("P", "Creating window");

    if (!glfwInit()) {
        Logger::Log("E", "GLFW Initialization failed in glfwInit");
        return false;
    }

    isGLFWInited = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        Logger::Log("E", "Creating window failed in glfwCreateWindow");
        return false;
    }

    glfwSetWindowUserPointer(window, this);

    return true;
}

Window& Window::GetInstance()
{
    static Window window;
    return window;
}

void Window::CloseWindow()
{
    Logger::Log("P", "Closing window");
    if (isGLFWInited) {
        glfwTerminate();
    }
}
