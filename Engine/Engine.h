#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include "Graphics.h"
#include "Logger.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "InterfaceManager.h"
#include "VisualScriptManager.h"
#include "InputManager.h"
#include "Timer.h"
#include "AssetDatabase.h"
#include "AssetSaver.h"
#include "Project.h"

//TODO: ADD SCRIPT SAVING / LOADING

class Engine
{
public:
	//main funcs
	ENGINE_API bool InitEngine(GLFWwindow* window);
	ENGINE_API void Update();
	ENGINE_API void Draw();
	ENGINE_API void CloseEngine();

private:
	//helpers
	void InitEntityTypes();
	void CalcFPSandMS();
	void UpdateCurrentScene();
	void PerformDeleteActions();
	void UpdateEngineWhenProjectIsNotOpened();
	void UpdateEngineWhenProjectIsOpened();
	void PerformSceneDeletion(std::string& projectDir, std::string& projectFile);
	void PerformTabDeletion(std::string& projectDir, std::string& projectFile);
	void HandleProjectOpeningOrCreation();
	bool HandleProjectCreation();
	bool HandleProjectOpeningWithPath();
	bool HandleProjectOpeningWithLatestProjects();
public:
	//getters
	ENGINE_API GLFWwindow* GetWindow() { return window; }

public:
	//getter for the instance
	ENGINE_API static Engine& GetInstance();

private:
	//singleton
	Engine() = default;
	~Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

private:
	//glfw
	GLFWwindow* window = nullptr;

	//imgui and imnodes
	ImGuiContext* context = nullptr;
	ImNodesContext* nodesContext = nullptr;

public:
	//store screen width and height
	int screenWidth = 0;
	int screenHeight = 0;

	//store frames per second(FPS) and elapsed time to draw next frame(ms)
	float FPS = 0;
	float ms = 0;

	//store all of entity types to use it when user add them to the scene
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>> entityTypes;
};

