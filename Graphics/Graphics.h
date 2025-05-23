#pragma once

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>

#include "Logger.h"

class Graphics
{
public:
	//main funcs
	GRAPHICS_API bool	      InitGraphics(GLFWwindow* window);
	GRAPHICS_API void	      ReleaseGraphics();
	GRAPHICS_API void	      CreateFramebuffer(int width, int height);
	GRAPHICS_API void	      RescaleFramebuffer(int width, int height);
	GRAPHICS_API unsigned int LoadTexture(const char* id, const char* path, int& width, int& height);
	GRAPHICS_API void		  UnloadTexture(unsigned int texture);
	GRAPHICS_API void		  UnloadMesh(unsigned int vao, unsigned int vboPos, unsigned int vboNormal, unsigned int vboUv);
	GRAPHICS_API unsigned int LoadMesh(const char* id, const char* path, int& verticeCount);
public:
	//getter for the instance
	GRAPHICS_API static Graphics& GetInstance();
private:
	//singleton
	Graphics() = default;
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
public:
	//callback funcs
	GRAPHICS_API void WindowSizeCallback(GLFWwindow* window, int width, int height);
	GRAPHICS_API static void StaticWindowSizeCallback(GLFWwindow* window, int width, int height) {
		Graphics* self = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
		if (self) {
			self->WindowSizeCallback(window, width, height);
		}
	}
public:
	//context
	GRAPHICS_API void Clear();
	GRAPHICS_API void BindFramebuffer()   { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }
	GRAPHICS_API void UnbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
public:
	//getters
	GRAPHICS_API GLFWwindow*  GetWindow()         { return window; }
	GRAPHICS_API unsigned int GetFrameBufferTex() { return frameBufferTex; }
private:
	//glfw
	GLFWwindow* window = nullptr;

	//framebuffer
	unsigned int FBO;
	unsigned int RBO;
	unsigned int frameBufferTex;
};