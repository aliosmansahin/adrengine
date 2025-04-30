#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

enum ShaderType {
	SHADER_2D,
};

class ShaderManager
{
public:
	//main funcs
	bool InitShaderManager();
	bool InitShaders(ShaderType shaderType);
	void ApplyTransformMatrix(const char* uniformName, glm::mat4 mat);
	void ApplyTexture();
	void UpdateProjectionMatrix(int windowWidth, int windowHeight, int cameraX, int cameraY);
	void ReleaseShaderManager();
	//getter for the instance of class
	static ShaderManager& GetInstance();
private:
	ShaderManager() = default;
	~ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
private:
	Shader* shader = nullptr;
	ShaderType shaderType = SHADER_2D;
	unsigned int program = 0;
};

