#pragma once

#include <GLFW/glfw3.h>

#include "Logger.h"

class Window
{
public:
	//main funcs
	~Window();
	bool CreateWindow(int width, int height, const char* title);
public:
	//getters
	bool ShouldClose() { return glfwWindowShouldClose(window); }
	void SwapBuffers() { return glfwSwapBuffers(window); }
	void PollEvents() { return glfwPollEvents(); }
	GLFWwindow* GetWindow() { return window; }
	//closing function
private:
	void CloseWindow();
private:
	//glfw
	GLFWwindow* window = nullptr;
	bool isGLFWInited = false;
};

