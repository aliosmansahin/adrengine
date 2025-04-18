#pragma once

#include "Shader.h"

enum ShaderType {
	SHADER_2D,
};

class ShaderManager
{
public:
	//main funcs
	~ShaderManager();
	bool InitShaderManager();
	bool InitShaders(ShaderType shaderType);

private:
	Shader* shader = nullptr;
	unsigned int program;
};

