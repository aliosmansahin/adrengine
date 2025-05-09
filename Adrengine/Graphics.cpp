#include "Graphics.h"

#include "stb_image.h"

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

unsigned int Graphics::LoadTexture(const char* id, const char* path, int& width, int& height)
{
    unsigned int texture;
    glGenTextures(1, &texture);                  // Texture ID olu�tur
    glBindTexture(GL_TEXTURE_2D, texture);       // Texture'u ba�la

    // Texture parametreleri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S (X) y�n�nde tekrar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T (Y) y�n�nde tekrar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // K���ltme filtresi
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // B�y�tme filtresi

    // G�rseli y�kle
    int nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        // RGBA m� RGB mi kontrol et
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Mipmap olu�tur
    }
    else {
        std::string str = "Could not load texture \"";
        str += path;
        str += "\"";

        Logger::Log("E", str.c_str());
        return -1;
    }
    stbi_image_free(data); // Belle�i serbest b�rak

    std::string str = "Loaded texture \"";
    str += path;
    str += "\"";

    Logger::Log("P", str.c_str());
    return texture;
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
