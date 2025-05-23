#pragma once

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include "Shader.h"

enum ShaderType {
	SHADER_2D,
	SHADER_3D,
	DEPTH,
	DEPTH_CUBE
};

class ShaderManager
{
public:
	//main funcs
	GRAPHICS_API bool       InitShaderManager();
	GRAPHICS_API bool       InitShaders(ShaderType shaderType);
	GRAPHICS_API void       UseShaders(ShaderType shaderType);
	GRAPHICS_API ShaderType GetCurrentType();
	GRAPHICS_API void		ApplyTransformMatrix(const char* uniformName, glm::mat4 mat);
	GRAPHICS_API void		ApplyUniformVec3(const char* uniformName, glm::vec3 vec);
	GRAPHICS_API void		ApplyUniformInt(const char* uniformName, int value);
	GRAPHICS_API void		ApplyUniformFloat(const char* uniformName, float value);
	GRAPHICS_API void		ApplyUniformMatrix(const char* uniformName, glm::mat4 mat);
	GRAPHICS_API void		ApplyTexture(const char* uniformName);
	GRAPHICS_API void		UpdateTransformMatrix2D(int windowWidth, int windowHeight, int cameraX, int cameraY); //CHANGE THE NAME OF FUNCTION FROM PROJECTION TO TRANSFORM
	GRAPHICS_API void		UpdateTransformMatrix3D(glm::vec3 eye, int windowWidth, int windowHeight, float cameraX, float cameraY, float cameraZ, float yaw, float pitch);
	GRAPHICS_API void		ReleaseShaderManager();
	//getter for the instance of class
	GRAPHICS_API static ShaderManager& GetInstance();
private:
	ShaderManager() = default;
	~ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
private:
	std::map<int, unsigned int> programs;
	unsigned int currentProgram;
	ShaderType currentType = SHADER_3D;
};

