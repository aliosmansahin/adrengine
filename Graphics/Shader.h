#pragma once

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include "glad/glad.h"
#include <fstream>
#include <sstream>

#include "Logger.h"

class Shader
{
public:
	//functions
	GRAPHICS_API bool		  CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = "");
	GRAPHICS_API void		  ReleaseShader();
	GRAPHICS_API unsigned int GetVertexShader()   { return vertexShader; }
	GRAPHICS_API unsigned int GetFragmentShader() { return fragmentShader; }
	GRAPHICS_API unsigned int GetGeometryShader() { return geometryShader; }
private:
	//each shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int geometryShader;
};