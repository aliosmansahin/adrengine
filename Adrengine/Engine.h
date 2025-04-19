#pragma once

#include "Graphics.h"
#include "Logger.h"
#include "EntityManager.h"
#include "InterfaceManager.h"

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
	//Graphics* graphics = nullptr; //The class graphics was become singleton
	//EntityManager* entityManager = nullptr; //The class entitymanager was become singleton
	InterfaceManager* interfaceManager = nullptr;
};

