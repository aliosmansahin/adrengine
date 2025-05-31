#include "pch.h"
#include "Shader.h"

/*
PURPOSE: Loads each shader from file and compile them
*/
bool Shader::CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath)
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
    vertexShader = adr_glCreateShader(GL_VERTEX_SHADER);
    adr_glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    adr_glCompileShader(vertexShader);

    //check if the vertex shader compilation succeed
    int success;
    char vertexShaderInfoLog[512];
    adr_glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        adr_glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
        Logger::Log("E", vertexShaderInfoLog);
        return false;
    }
    
    Logger::Log("P", "Loading fragment shader");

    //load the fragment shader source
    std::ifstream fragmentShaderFile(fragmentShaderPath);
    if (!fragmentShaderFile.is_open()) {
        std::string str = "There is not a file which has name \"";
        str += fragmentShaderPath;
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
    fragmentShader = adr_glCreateShader(GL_FRAGMENT_SHADER);
    adr_glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    adr_glCompileShader(fragmentShader);

    //check if the fragment shader compilation succeed
    char fragmentShaderInfoLog[512];
    adr_glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        adr_glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
        Logger::Log("E", fragmentShaderInfoLog);
        return false;
    }

    //check there is a geometry shader that will be compiled
    if (geometryShaderPath != "") {
        Logger::Log("P", "Loading geometry shader");

        //load the geometry shader source
        std::ifstream geometryShaderFile(geometryShaderPath);
        if (!geometryShaderFile.is_open()) {
            std::string str = "There is not a file which has name \"";
            str += geometryShaderPath;
            str += "\"";
            Logger::Log("E", str.c_str());
            return false;
        }
        std::stringstream geometryStream;
        geometryStream << geometryShaderFile.rdbuf();
        geometryShaderFile.close();
        std::string geometryShaderString = geometryStream.str();
        const char* geometryShaderSource = geometryShaderString.c_str();

        //create a geometry shader
        geometryShader = adr_glCreateShader(GL_GEOMETRY_SHADER);
        adr_glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        adr_glCompileShader(geometryShader);

        //check if the geometry shader compilation succeed
        char geometryShaderInfoLog[512];
        adr_glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            adr_glGetShaderInfoLog(geometryShader, 512, NULL, geometryShaderInfoLog);
            Logger::Log("E", geometryShaderInfoLog);
            return false;
        }
    }
    else //otherwise geomerty shader won't store anything
        geometryShader = -1;

    return true;
}

/*
PURPOSE: Releases all shader types
*/
void Shader::ReleaseShader()
{
    adr_glDeleteShader(vertexShader);
    adr_glDeleteShader(fragmentShader);
    if(geometryShader != -1)
        adr_glDeleteShader(geometryShader);
}
