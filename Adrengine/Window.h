#pragma once

#include <GLFW/glfw3.h>

#include "Logger.h"

class Window
{
public:
	//main funcs
	bool CreateWindow(int width, int height, const char* title);
	void CloseWindow();
public:
	//getters
	bool ShouldClose() { return glfwWindowShouldClose(window); }
	void SwapBuffers() { return glfwSwapBuffers(window); }
	void PollEvents() { return glfwPollEvents(); }
	GLFWwindow* GetWindow() { return window; }
public:
	//getter for the instance
	static Window& GetInstance();
private:
	//singleton
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	//glfw
	GLFWwindow* window = nullptr;
	bool isGLFWInited = false;
};

