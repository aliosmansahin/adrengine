#include "pch.h"
#include "ShaderManager.h"

/*
PURPOSE: Initializes vertex, fragment and (if it's exists) geomerty shaders
*/
bool ShaderManager::InitShaders(Utils::ShaderType shaderType)
{
    //create a shader instance
    Shader* shader = nullptr;

    //initialize shaders as the type
    if (shaderType == Utils::SHADER_2D) {
        Logger::Log("P", "Initializing SHADER_2D");
        shader = new Shader();
        if (!shader->CreateShader("shaders/2DVertexShader.glsl", "shaders/2DFragmentShader.glsl")) {
            shader->ReleaseShader();
            delete shader;
            return false;
        }
    }
    else if (shaderType == Utils::SHADER_3D) {
        Logger::Log("P", "Initializing SHADER_3D");
        shader = new Shader();
        if (!shader->CreateShader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl")) {
            shader->ReleaseShader();
            delete shader;
            return false;
        }
    }
    else if (shaderType == Utils::DEPTH) {
        Logger::Log("P", "Initializing DEPTH");
        shader = new Shader();
        if (!shader->CreateShader("shaders/DepthVertexShader.glsl", "shaders/DepthFragmentShader.glsl")) {
            shader->ReleaseShader();
            delete shader;
            return false;
        }
    }
    else if (shaderType == Utils::DEPTH_CUBE) {
        Logger::Log("P", "Initializing DEPTH_CUBE");
        shader = new Shader();
        if (!shader->CreateShader("shaders/DepthCubeVertexShader.glsl", "shaders/DepthCubeFragmentShader.glsl", "shaders/DepthCubeGeometryShader.glsl")) {
            shader->ReleaseShader();
            delete shader;
            return false;
        }
    }
    else if (shaderType == Utils::SHADER_INSPECT_TILE) {
        Logger::Log("P", "Initializing SHADER_INSPECT_TILE");
        shader = new Shader();
        if (!shader->CreateShader("shaders/TileMapInspectVertexShader.glsl", "shaders/TileMapInspectFragmentShader.glsl")) {
            shader->ReleaseShader();
            delete shader;
            return false;
        }
    }
    else {
        Logger::Log("E", "Invalid shader type");
        return false;
    }

    //create a shader program
    unsigned int program = adr_glCreateProgram();

    //attach and link
    adr_glAttachShader(program, shader->GetVertexShader());
    adr_glAttachShader(program, shader->GetFragmentShader());
    if(shader->GetGeometryShader() != -1)
        adr_glAttachShader(program, shader->GetGeometryShader());

    adr_glLinkProgram(program);

    //check linking status
    int success;
    char infoLog[512];
    adr_glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        adr_glGetProgramInfoLog(program, 512, NULL, infoLog);
        Logger::Log("E", infoLog);

        shader->ReleaseShader();
        delete shader;

        adr_glDeleteProgram(program);
        return false;
    }

    //sets the program as current and add it to all programs
    currentProgram = program;
    currentType = shaderType;
    programs[shaderType] = program;

    //releases shaders
    shader->ReleaseShader();
    delete shader;

    return true;
}

/*
PURPOSE: Set the program as current
*/
void ShaderManager::UseShaders(Utils::ShaderType shaderType)
{
    if (currentType != shaderType) {
        currentProgram = programs[shaderType];
        currentType = shaderType;
        adr_glUseProgram(currentProgram);
    }
}

/*
PURPOSE: Gets type of current shader
*/
Utils::ShaderType ShaderManager::GetCurrentType()
{
    return currentType;
}

/*
PURPOSE: Sends vector to uniform
*/
void ShaderManager::ApplyUniformVec3(const char* uniformName, glm::vec3 vec)
{
    unsigned int transformLoc = adr_glGetUniformLocation(currentProgram, uniformName);
    adr_glUniform3f(transformLoc, vec.x, vec.y, vec.z);
}

/*
PURPOSE: Sends int to uniform
*/
void ShaderManager::ApplyUniformInt(const char* uniformName, int value)
{
    unsigned int transformLoc = adr_glGetUniformLocation(currentProgram, uniformName);
    adr_glUniform1i(transformLoc, value);
}

/*
PURPOSE: Sends float to uniform
*/
void ShaderManager::ApplyUniformFloat(const char* uniformName, float value)
{
    unsigned int transformLoc = adr_glGetUniformLocation(currentProgram, uniformName);
    adr_glUniform1f(transformLoc, value);
}

/*
PURPOSE: Sends matrix to uniform
*/
void ShaderManager::ApplyUniformMatrix(const char* uniformName, glm::mat4 mat)
{
    unsigned int textureLoc = adr_glGetUniformLocation(currentProgram, uniformName);
    adr_glUniformMatrix4fv(textureLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

/*
PURPOSE: Sends bool to uniform
*/
GRAPHICS_API void ShaderManager::ApplyUniformBool(const char* uniformName, bool value)
{
    unsigned int transformLoc = adr_glGetUniformLocation(currentProgram, uniformName);
    adr_glUniform1i(transformLoc, value);
}

/*
PURPOSE: Sends texture to uniform
*/
void ShaderManager::ApplyTexture(const char* uniformName)
{
    unsigned int textureLoc = adr_glGetUniformLocation(currentProgram, uniformName);
    adr_glUniform1i(textureLoc, 0);
}

/*
PURPOSE: Updates 2d projection and view matrices and send them to uniforms,
    the model matrix will be sent from each entities
*/
void ShaderManager::UpdateTransformMatrix2D(int windowWidth, int windowHeight, int cameraX, int cameraY, bool invertY)
{
    glm::mat4 projection = GetProjectionMatrix2D(windowWidth, windowHeight, invertY);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, 0));

    ApplyUniformMatrix("uProjection", projection);
    ApplyUniformMatrix("uView", translate);
}

/*
PURPOSE: Updates 3d projection and view matrices and send them to uniforms,
    the model matrix will be sent from each entities
*/
void ShaderManager::UpdateTransformMatrix3D(glm::vec3 eye, int windowWidth, int windowHeight, float cameraX, float cameraY, float cameraZ)
{
    glm::vec3 pos = glm::vec3(cameraX, cameraY, cameraZ);
    glm::vec3 target = eye + pos;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 look = glm::lookAt(pos, target, up);

    float aspect = (float)windowWidth / (float)windowHeight;
    float fov = 70.0f;

    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, 0.01f, 1000.0f);

    ApplyUniformMatrix("uView", look);
    ApplyUniformMatrix("uProjection", proj);
}

/*
PURPOSE: Calculates projection matrix for 2d
*/
GRAPHICS_API glm::mat4 ShaderManager::GetProjectionMatrix2D(int windowWidth, int windowHeight, bool invertY)
{
    float aspect = (float)windowWidth / (float)windowHeight;

    float orthoWidth = 800.0f;
    float orthoHeight = orthoWidth / aspect;

    glm::mat4 projection;

    if (invertY)
        projection = glm::ortho(0.0f, orthoWidth, 0.0f, orthoHeight, -100.0f, 100.0f);
    else
        projection = glm::ortho(0.0f, orthoWidth, orthoHeight, 0.0f, -100.0f, 100.0f);

    return projection;
}

/*
PURPOSE: Release all shaders and the manager
*/
void ShaderManager::ReleaseShaderManager()
{
    //Delete each program
    for(auto& iter : programs)
        adr_glDeleteProgram(iter.second);

    //set the current program to nothing
    currentProgram = -1;

    //logger
    Logger::Log("P", "Cleared shader manager");
}

/*
PURPOSE: Gets the instance
*/
ShaderManager& ShaderManager::GetInstance()
{
    static ShaderManager instance;
    return instance;
}

/*
PURPOSE: Initialize the manager
*/
bool ShaderManager::InitShaderManager()
{
    //initilize shader manager
    Logger::Log("P", "Initializing shader manager");

    return true;
}
