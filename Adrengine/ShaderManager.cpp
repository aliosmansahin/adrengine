#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
    if (shader) delete shader;
    glDeleteProgram(program);
    Logger::Log("P", "Cleared shader manager");
}

bool ShaderManager::InitShaders(ShaderType shaderType)
{
    //load shaders
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

bool ShaderManager::InitShaderManager()
{   
    program = glCreateProgram();
    return true;
}
