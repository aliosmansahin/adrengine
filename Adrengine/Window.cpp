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

        glfwSetWindowIcon(window, 1, &glfwImage); // set the icon
        stbi_image_free(image); // free the memory
    }
    else {
        Logger::Log("E", "Could not load icon of program, using default");
    }

    //make the window's context current and check if it's corrent
    glfwMakeContextCurrent(window);
    
    //Check for the context
    if (!glfwGetCurrentContext()) {
        Logger::Log("E", "No current OpenGL context");
        return false;
    }

    //load opengl functions
    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (version == 0) {
        Logger::Log("E", "Starting engine failed in gladLoadGL");
        return false;
    }
    
    /*
        INFO: Glad functions that are loaded in exe don't work corrently in DLLs because of some access violation error,
        So we are loading them in exe, after that the funcion pointers will be passed to GladWrapper in order to use it in other DLLs
    */
    LoadGLFunctions();

    Logger::Log("P", "Loaded OpenGL Version 3.3");

    //handle window size callback
    glfwSetWindowSizeCallback(window, Graphics::StaticWindowSizeCallback);

    return true;
}

/*
PURPOSE: Passes glad function pointers to GladWrapper
*/
void Window::LoadGLFunctions()
{
    adr::adr_glClear = glad_glClear;
    adr::adr_glClearColor = glad_glClearColor;
    adr::adr_glGenFramebuffers = glad_glGenFramebuffers;
    adr::adr_glBindFramebuffer = glad_glBindFramebuffer;
    adr::adr_glGenBuffers = glad_glGenBuffers;
    adr::adr_glBindBuffer = glad_glBindBuffer;
    adr::adr_glBufferData = glad_glBufferData;
    adr::adr_glEnableVertexAttribArray = glad_glEnableVertexAttribArray;
    adr::adr_glVertexAttribPointer = glad_glVertexAttribPointer;
    adr::adr_glGenVertexArrays = glad_glGenVertexArrays;
    adr::adr_glBindVertexArray = glad_glBindVertexArray;
    adr::adr_glDeleteFramebuffers = glad_glDeleteFramebuffers;
    adr::adr_glDeleteRenderbuffers = glad_glDeleteRenderbuffers;
    adr::adr_glDeleteTextures = glad_glDeleteTextures;
    adr::adr_glTexImage2D = glad_glTexImage2D;
    adr::adr_glTexParameteri = glad_glTexParameteri;
    adr::adr_glGenerateMipmap = glad_glGenerateMipmap;
    adr::adr_glCheckFramebufferStatus = glad_glCheckFramebufferStatus;
    adr::adr_glRenderbufferStorage = glad_glRenderbufferStorage;
    adr::adr_glFramebufferTexture2D = glad_glFramebufferTexture2D;
    adr::adr_glFramebufferRenderbuffer = glad_glFramebufferRenderbuffer;
    adr::adr_glGenTextures = glad_glGenTextures;
    adr::adr_glBindTexture = glad_glBindTexture;
    adr::adr_glGenRenderbuffers = glad_glGenRenderbuffers;
    adr::adr_glBindRenderbuffer = glad_glBindRenderbuffer;
    adr::adr_glDeleteBuffers = glad_glDeleteBuffers;
    adr::adr_glDeleteVertexArrays = glad_glDeleteVertexArrays;
    adr::adr_glViewport = glad_glViewport;
    adr::adr_glCreateShader = glad_glCreateShader;
    adr::adr_glShaderSource = glad_glShaderSource;
    adr::adr_glCompileShader = glad_glCompileShader;
    adr::adr_glGetShaderiv = glad_glGetShaderiv;
    adr::adr_glGetShaderInfoLog = glad_glGetShaderInfoLog;
    adr::adr_glDeleteShader = glad_glDeleteShader;
    adr::adr_glCreateProgram = glad_glCreateProgram;
    adr::adr_glAttachShader = glad_glAttachShader;
    adr::adr_glLinkProgram = glad_glLinkProgram;
    adr::adr_glGetProgramiv = glad_glGetProgramiv;
    adr::adr_glGetProgramInfoLog = glad_glGetProgramInfoLog;
    adr::adr_glUseProgram = glad_glUseProgram;
    adr::adr_glGetUniformLocation = glad_glGetUniformLocation;
    adr::adr_glUniformMatrix4fv = glad_glUniformMatrix4fv;
    adr::adr_glUniform3f = glad_glUniform3f;
    adr::adr_glUniform1i = glad_glUniform1i;
    adr::adr_glUniform1f = glad_glUniform1f;
    adr::adr_glDeleteProgram = glad_glDeleteProgram;
    adr::adr_glEnable = glad_glEnable;
    adr::adr_glBlendFunc = glad_glBlendFunc;
    adr::adr_glCullFace = glad_glCullFace;
    adr::adr_glActiveTexture = glad_glActiveTexture;
    adr::adr_glDisable = glad_glDisable;
    adr::adr_glDrawElements = glad_glDrawElements;
    adr::adr_glDrawArrays = glad_glDrawArrays;
    adr::adr_glTexParameterfv = glad_glTexParameterfv;
    adr::adr_glDrawBuffer = glad_glDrawBuffer;
    adr::adr_glReadBuffer = glad_glReadBuffer;
    adr::adr_glFramebufferTexture = glad_glFramebufferTexture;
    adr::adr_glBlendFuncSeparate = glad_glBlendFuncSeparate;
    adr::adr_glDepthMask = glad_glDepthMask;
    adr::adr_glDepthFunc = glad_glDepthFunc;
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
    //Window size callback
    glfwSetWindowSizeCallback(window, nullptr);

    //GLFW termination
    if (isGLFWInited) {
        glfwTerminate();
    }

    //Log
    Logger::Log("P", "Closed window");
}
