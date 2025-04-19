#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "ShaderManager.h"

class Graphics
{
public:
	//main funcs
	bool InitGraphics(GLFWwindow* window);
	void ReleaseGraphics();
public:
	//getter for the instance
	static Graphics& GetInstance();
private:
	//singleton
	Graphics() = default;
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
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