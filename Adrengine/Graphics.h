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
	void CreateFramebuffer(int width, int height);
	void RescaleFramebuffer(int width, int height);
	bool LoadTexture(const char* id, const char* path);
	unsigned int GetTexture(std::string id);
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
	void BindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }
	void UnbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
public:
	//getters
	GLFWwindow* GetWindow() { return window; }
	unsigned int GetFrameBufferTex() { return frameBufferTex; }
private:
	//glfw
	GLFWwindow* window = nullptr;
	//framebuffer
	unsigned int FBO;
	unsigned int RBO;
	unsigned int frameBufferTex;
	std::unordered_map<std::string, unsigned int> textures;
};