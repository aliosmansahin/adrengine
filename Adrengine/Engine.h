#pragma once

#include "Graphics.h"
#include "Logger.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "InterfaceManager.h"
#include "VisualScriptManager.h"
#include "InputManager.h"

class Scene;
struct Tab;

class Engine
{
public:
	//main funcs
	bool InitEngine(GLFWwindow* window);
	void Update();
	void Draw();
	void CloseEngine();
	void LoadProject();
	void SaveProject();
	void SyncTabAndScene(Scene* scene, Tab* tab, std::string currentSceneId);
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
public:
	//project specifications
	std::string projectName = "project";
	std::string projectPath = "C:\\Users\\osman\\OneDrive\\Desktop\\";
	int screenWidth = 0;
	int screenHeight = 0;

	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>> entityTypes;
};

