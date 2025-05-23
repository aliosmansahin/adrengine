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

//TODO: ADD SCRIPT SAVING / LOADING

class Scene;

class Engine
{
public:
	//main funcs
	ENGINE_API bool InitEngine(GLFWwindow* window);
	ENGINE_API void InitEntityTypes();
	ENGINE_API void Update();
	ENGINE_API void Draw();
	ENGINE_API void CloseEngine();
	ENGINE_API void LoadProject();
	ENGINE_API void SaveProject();
	ENGINE_API void CalcFPSandMS();
	ENGINE_API void UpdateCurrentScene();
	ENGINE_API void PerformDeleteActions();
	ENGINE_API void RequestGetVideoMode();
	ENGINE_API void AnswerGetVideoMode(int width, int height);
	ENGINE_API bool GetRequestGetVideoMode();
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
	bool requestGetVideoMode = false;
public:
	//project specifications
	std::string projectName = "project";
	std::string projectPath = "C:\\Users\\osman\\OneDrive\\Desktop\\";
	
	//store screen width and height
	int screenWidth = 0;
	int screenHeight = 0;

	//store frames per second(FPS) and elapsed time to draw next frame(ms)
	float FPS = 0;
	float ms = 0;

	//store all of entity types to use it when user add them to the scene
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>> entityTypes;
};

