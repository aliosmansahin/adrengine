#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Window::CreateWindow(int width, int height, const char* title)
{
    /* Initialize the library */
    Logger::Log("P", Localization::Get().GetString("creating_window_text"));

    if (!glfwInit()) {
        Logger::Log("E", "GLFW Initialization failed in glfwInit");
        return false;
    }

    isGLFWInited = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        Logger::Log("E", "Creating window failed in glfwCreateWindow");
        return false;
    }
    
    int imageWidth, imageHeight, channels;
    unsigned char* image = stbi_load("icon.png", &imageWidth, &imageHeight, &channels, 4); // RGBA

    if (image) {
        GLFWimage glfwImage;
        glfwImage.width = imageWidth;
        glfwImage.height = imageHeight;
        glfwImage.pixels = image;

        glfwSetWindowIcon(window, 1, &glfwImage); // ikonu ayarla
        stbi_image_free(image); // bellek temizliði
    }
    else {
        Logger::Log("E", "Could not load icon of program, using default");
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
