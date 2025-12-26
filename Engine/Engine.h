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
#include "BulletPhysics.h"

#include "interfaces/IEngine/IEngine.h"
#include "interfaces/IProject/IProject.h"

#include "ServiceLocator.h"

class Engine : public IEngine
{
public:
	//main funcs
	ENGINE_API bool InitEngine(GLFWwindow* window) override;
	ENGINE_API void Update() override;
	ENGINE_API void Draw() override;
	ENGINE_API void CloseEngine() override;

	//Factory
	ENGINE_API std::shared_ptr<IEntityManager> CreateEntityManager() override;
	ENGINE_API std::shared_ptr<IEntity> CreateEntity(std::string entityType) override;
	ENGINE_API std::shared_ptr<IScene> CreateScene() override;
	ENGINE_API std::shared_ptr<IPhysics> CreatePhysics() override;
	ENGINE_API std::shared_ptr<IRigidBody> CreateRigidBody() override;

	//Getters
	ENGINE_API std::unordered_map<std::string, std::pair<std::shared_ptr<IEntity>, std::shared_ptr<IEntityParams>>>& GetEntityTypes() override;
	ENGINE_API std::pair<int, int> GetScreenSize() override;
	ENGINE_API std::pair<float, float> GetFPSandMS() override;
	ENGINE_API std::pair<int, int> GetMainWindowSize() override;

	//Close editor
	ENGINE_API void CloseEditor() override { glfwSetWindowShouldClose(window, true); };

public:
	//Singleton
	static Engine& GetInstance()
	{
		static Engine instance;
		return instance;
	}

private:
	//helpers
	void InitEntityTypes();
	void CalcFPSandMS();
	void UpdateCurrentScene();
	void PerformDeleteActions();
	void UpdateEngineWhenProjectIsNotOpened();
	void UpdateEngineWhenProjectIsOpened();
	void PerformSceneDeletion();
	void PerformTabDeletion();
	void HandleProjectOpeningOrCreation();
	bool HandleProjectCreation();
	bool HandleProjectOpeningWithPath();
	bool HandleProjectOpeningWithLatestProjects();

	void SetMainWindowSize(GLFWwindow* mainWindow);

public:
	//getters
	ENGINE_API GLFWwindow* GetWindow() { return window; }

public:
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

private:
	//store screen width and height
	int screenWidth = 0;
	int screenHeight = 0;

	//store main window width and height
	int mainWindowWidth = 0;
	int mainWindowHeight = 0;

	//store frames per second(FPS) and elapsed time to draw next frame(ms)
	float FPS = 0;
	float ms = 0;

	//store all of entity types to use it when user add them to the scene
	std::unordered_map<std::string, std::pair<std::shared_ptr<IEntity>, std::shared_ptr<IEntityParams>>> entityTypes;
};

