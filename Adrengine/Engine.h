#pragma once

#include "Graphics.h"
#include "Logger.h"
#include "EntityManager.h"
#include "InterfaceManager.h"

class Engine
{
public:
	//main funcs
	bool InitEngine(GLFWwindow* window);
	void Update();
	void Draw();
	void CloseEngine();
public:
	//getters
	GLFWwindow* GetWindow() { return window; }
public:
	//getter for the instance
	static Engine& GetInstance();
private:
	//singleton
	Engine() = default;
	~Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
private:
	//glfw
	GLFWwindow* window = nullptr;
	//instances for each engine
	//Graphics* graphics = nullptr; //The class graphics was become singleton
	//EntityManager* entityManager = nullptr; //The class entitymanager was become singleton
	//InterfaceManager* interfaceManager = nullptr; //The class interfacemanager was become singleton
};

