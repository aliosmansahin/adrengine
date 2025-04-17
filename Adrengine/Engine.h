#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Logger.h"

class Engine
{
public:
	//main funcs
	~Engine();
	bool InitEngine(GLFWwindow* window);
	void Update();
	void Draw();
public:
	//getters
	GLFWwindow* GetWindow() { return window; }
private:
	//glfw
	GLFWwindow* window = nullptr;
};

