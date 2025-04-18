#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "ShaderManager.h"

class Graphics
{
public:
	//main funcs
	~Graphics();
	bool InitGraphics(GLFWwindow* window);
public:
	//context
	void Clear();
public:
	//getters
	GLFWwindow* GetWindow() { return window; }
private:
	//glfw
	GLFWwindow* window = nullptr;
private:
	ShaderManager* shaderManager = nullptr;

};