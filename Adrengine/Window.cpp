#include "Window.h"
#include "InputManager.h"
#include "Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

/*
PURPOSE: To create a window
*/
bool Window::CreateWindow(int width, int height, const char* title)
{
    //Log stuff
    Logger::Log("P", Localization::GetString("creating_window_text"));

    //Initializes glfw
    if (!glfwInit()) {
        Logger::Log("E", "GLFW Initialization failed in glfwInit");
        return false;
    }

    isGLFWInited = true;

    //GLFW and OpenGL settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    
    //Create a window and pass it to a pointer
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    
    //Load window icon and set it to the window
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

    //FIXME:
    //this class will be window pointer, i might delete it
    glfwSetWindowUserPointer(window, this);

    //make the window's context current and check if it's corrent
    glfwMakeContextCurrent(window);

    if (!glfwGetCurrentContext()) {
        Logger::Log("E", "No current OpenGL context");
        return false;
    }

    //handle window size callback
    glfwSetWindowSizeCallback(window, Graphics::StaticWindowSizeCallback);

    /*
        FIXME:
        They have to be in the inputmanager but due to some errors, i removed them.
        If i can solve it, i will move them back
    */
    //glfwSetKeyCallback(window, InputManager::KeyCallback);
    //glfwSetCursorPosCallback(window, InputManager::CursorPositionCallback);

    return true;
}

/*
PURPOSE: To handle glfw using requests
FIXME:
This function will be removed once the glfw error was fixed
*/
void Window::HandleGLFWRequests()
{
    if (Engine::GetInstance().GetRequestGetVideoMode()) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        Engine::GetInstance().AnswerGetVideoMode(mode->width, mode->height);
    }
    if (InputManager::GetInstance().GetRequestSetMouseVisibility()) {
        bool visible = InputManager::GetInstance().GetMouseVisibility();
        if (visible) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if (InputManager::GetInstance().GetRequestGetMouseButtons()) {
        std::unordered_map<int, bool> currentMouseButtons;
        for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
            currentMouseButtons[i] = glfwGetMouseButton(window, i);
        }
        InputManager::GetInstance().AnswerGetMouseButtons(currentMouseButtons);
    }
    if (InputManager::GetInstance().GetRequestGetKeys()) {
        std::unordered_map<int, bool> currentKeys;
        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
            int state = glfwGetKey(window, key);
            currentKeys[key] = (state == GLFW_PRESS || state == GLFW_REPEAT);
        }
        InputManager::GetInstance().AnswerGetKeys(currentKeys);
    }
    if (InputManager::GetInstance().GetRequestGetMousePos()) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        InputManager::GetInstance().AnswerGetMousePos((int)xpos, (int)ypos);
    }
}

//PURPOSE: To get the instance of window singleton class
Window& Window::GetInstance()
{
    static Window window;
    return window;
}

//To release window and close it
void Window::CloseWindow()
{
    glfwSetKeyCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    if (isGLFWInited) {
        glfwTerminate();
    }
    Logger::Log("P", "Closed window");
}
