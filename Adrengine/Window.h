#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glad_wrapper.h"

#include "Logger.h"
#include "Localization.h"

class Window
{
public:
	//main funcs
	bool		   CreateWindow(int width, int height, const char* title);
	void		   CloseWindow();

public:			   
	//getters and helpers
	bool		   ShouldClose() { return glfwWindowShouldClose(window); }
	void		   SwapBuffers() { return glfwSwapBuffers(window); }
	void		   PollEvents() { return glfwPollEvents(); }
	GLFWwindow*	   GetWindow() { return window; }

public:
	//getter for the instance
	static Window& GetInstance();

private:
	//private function which will be used by Window class
	void		   LoadGLFunctions();
	void		   SetIconOfWindow();
private:
	//singleton
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	//glfw
	GLFWwindow* window = nullptr;
	bool isGLFWInited = false; //stores if glfw is initialized
};

