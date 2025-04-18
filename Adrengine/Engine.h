#pragma once

#include "Graphics.h"
#include "Logger.h"
#include "EntityManager.h"

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
	//instances for each engine
	Graphics* graphics = nullptr;
	EntityManager* entityManager = nullptr;
};

