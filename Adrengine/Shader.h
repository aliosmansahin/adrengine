#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>

#include "Logger.h"

class Shader
{
public:
	bool CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void ReleaseShader();
	unsigned int GetVertexShader() { return vertexShader; }
	unsigned int GetFragmentShader() { return fragmentShader; }
private:
	unsigned int vertexShader;
	unsigned int fragmentShader;

};