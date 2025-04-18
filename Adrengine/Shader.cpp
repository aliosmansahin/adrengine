#include "Shader.h"

bool Shader::CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    Logger::Log("P", "Loading vertex shader");

    //load the vertex shader source
    std::ifstream vertexShaderFile(vertexShaderPath);
    if (!vertexShaderFile.is_open()) {
        std::string str = "There is not a file which has name \"";
        str += vertexShaderPath;
        str += "\"";
        Logger::Log("E", str.c_str());
        return false;
    }

    std::stringstream vertexStream;
    vertexStream << vertexShaderFile.rdbuf();
    vertexShaderFile.close();
    std::string vertexShaderString = vertexStream.str();
    const char* vertexShaderSource = vertexShaderString.c_str();

    //create a vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //check if the vertex shader compilation succeed
    int success;
    char vertexShaderInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
        Logger::Log("E", vertexShaderInfoLog);
        return false;
    }
    
    Logger::Log("P", "Loading fragment shader");

    //load the fragment shader source
    std::ifstream fragmentShaderFile(fragmentShaderPath);
    if (!fragmentShaderFile.is_open()) {
        std::string str = "There is not a file which has name \"";
        str += vertexShaderPath;
        str += "\"";
        Logger::Log("E", str.c_str());
        return false;
    }
    std::stringstream fragmentStream;
    fragmentStream << fragmentShaderFile.rdbuf();
    fragmentShaderFile.close();
    std::string fragmentShaderString = fragmentStream.str();
    const char* fragmentShaderSource = fragmentShaderString.c_str();

    //create a fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //check if the fragment shader compilation succeed
    char fragmentShaderInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
        Logger::Log("E", fragmentShaderInfoLog);
        return false;
    }

    return true;
}

void Shader::ReleaseShader()
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
