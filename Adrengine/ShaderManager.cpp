#include "ShaderManager.h"

bool ShaderManager::InitShaders(ShaderType shaderType)
{
    //load shaders
    this->shaderType = shaderType;

    if (shaderType == SHADER_2D) {
        Logger::Log("P", "Initializing SHADER_2D");
        shader = new Shader();
        if (!shader->CreateShader("VertexShader.glsl", "FragmentShader.glsl"))
            return false;
    }
    else {
        Logger::Log("E", "Invalid shader type");
        return false;
    }

    //attach and link
    glAttachShader(program, shader->GetVertexShader());
    glAttachShader(program, shader->GetFragmentShader());
    glLinkProgram(program);

    //check linking status
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        Logger::Log("E", infoLog);
        return false;
    }

    glUseProgram(program);

    shader->ReleaseShader();

    return true;
}

void ShaderManager::ApplyTransformMatrix(const char* uniformName, glm::mat4 mat)
{
    unsigned int transformLoc = glGetUniformLocation(program, uniformName);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderManager::UpdateProjectionMatrix(int windowWidth, int windowHeight)
{
    float aspect = (float)windowWidth / (float)windowHeight;

    float orthoWidth = 800.0f;
    float orthoHeight = orthoWidth / aspect;

    glm::mat4 projection = glm::ortho(0.0f, orthoWidth, orthoHeight, 0.0f);

    ApplyTransformMatrix("uProjection", projection);
}

void ShaderManager::ReleaseShaderManager()
{
    if (shader) delete shader;
    glDeleteProgram(program);
    Logger::Log("P", "Cleared shader manager");
}

ShaderManager& ShaderManager::GetInstance()
{
    static ShaderManager instance;
    return instance;
}

bool ShaderManager::InitShaderManager()
{   
    program = glCreateProgram();
    return true;
}
