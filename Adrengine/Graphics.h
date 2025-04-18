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
	//callback funcs
	void WindowSizeCallback(GLFWwindow* window, int width, int height);
	static void StaticWindowSizeCallback(GLFWwindow* window, int width, int height) {
		Graphics* self = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
		if (self) {
			self->WindowSizeCallback(window, width, height);
		}
	}
public:
	//context
	void Clear();
public:
	//getters
	GLFWwindow* GetWindow() { return window; }
private:
	//glfw
	GLFWwindow* window = nullptr;

};