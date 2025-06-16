#include "pch.h"
#include "Shader.h"

/*
PURPOSE: Loads each shader from file and compile them
*/
bool Shader::CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath)
{
    Logger::Log("P", "Loading vertex shader");

    if (!LoadShader(GL_VERTEX_SHADER, vertexShaderPath, vertexShader))
        return false;
    
    Logger::Log("P", "Loading fragment shader");

    if (!LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath, fragmentShader))
        return false;

    //check there is a geometry shader that will be compiled
    if (geometryShaderPath != "") {
        Logger::Log("P", "Loading geometry shader");

        if (!LoadShader(GL_GEOMETRY_SHADER, geometryShaderPath, geometryShader))
            return false;
    }

    return true;
}

GRAPHICS_API bool Shader::LoadShader(unsigned int shaderTypeAsOpenGL, const char* shaderPath, unsigned int& shader)
{
    //load the shader source
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        std::string str = "There is not a file which has name \"";
        str += shaderPath;
        str += "\"";
        Logger::Log("E", str.c_str());
        return false;
    }

    std::stringstream stream;
    stream << shaderFile.rdbuf();
    shaderFile.close();
    std::string shaderString = stream.str();
    const char* shaderSource = shaderString.c_str();

    //create a vertex shader
    shader = adr_glCreateShader(shaderTypeAsOpenGL);
    adr_glShaderSource(shader, 1, &shaderSource, NULL);
    adr_glCompileShader(shader);

    //check if the vertex shader compilation succeed
    int success;
    char shaderInfoLog[512];
    adr_glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        adr_glGetShaderInfoLog(shader, 512, NULL, shaderInfoLog);
        Logger::Log("E", shaderInfoLog);
        return false;
    }

    return true;
}

/*
PURPOSE: Releases all shader types
*/
void Shader::ReleaseShader()
{
    if(vertexShader != -1)
        adr_glDeleteShader(vertexShader);
    if(fragmentShader = -1)
        adr_glDeleteShader(fragmentShader);
    if(geometryShader != -1)
        adr_glDeleteShader(geometryShader);
}
