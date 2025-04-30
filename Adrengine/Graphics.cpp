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
    
    //window size callback
    glfwSetWindowSizeCallback(window, StaticWindowSizeCallback);

    return true;
}

void Graphics::ReleaseGraphics()
{
    ShaderManager::GetInstance().ReleaseShaderManager();
    Logger::Log("P", "Cleared graphics");
}

void Graphics::CreateFramebuffer(int width, int height)
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &frameBufferTex);
    glBindTexture(GL_TEXTURE_2D, frameBufferTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::Log("E", "Framebuffer is not complete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Graphics::RescaleFramebuffer(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, frameBufferTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

Graphics& Graphics::GetInstance()
{
    static Graphics graphics;
    return graphics;
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
