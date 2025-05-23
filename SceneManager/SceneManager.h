#pragma once

#ifdef SCENEMANAGER_EXPORTS 
#define SCENEMANAGER_API __declspec(dllexport)
#else
#define SCENEMANAGER_API __declspec(dllimport)
#endif

#include "Scene.h"

class Scene;
enum SceneType;

//Tab objects
enum TabType {
	SceneEditor,
	VisualScriptEditor
};

struct Tab {
	std::string id;
	TabType tabType;
};

//Project json creation
SCENEMANAGER_API nlohmann::json CreateProjectJson(
	std::string openedTabId,
	std::map<std::string, std::string>& scenes,
	std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts,
	std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs);

class SceneManager
{
public:
	//manager functions
	SCENEMANAGER_API bool				  InitializeManager();
	SCENEMANAGER_API void				  ClearManager();

	//scene functions
	SCENEMANAGER_API bool				  CreateScene(
		SceneType sceneType,
		std::string& projectDir,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		Tab*& openedTab,
		std::string& selectedTabId,
		std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts
	);
	SCENEMANAGER_API Scene*				  LoadScene(std::string sceneId, std::string& projectDir,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes
		);
	SCENEMANAGER_API bool				  CloseScene(std::string sceneId, std::string& projectDir,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		Tab*& openedTab,
		std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts);
	SCENEMANAGER_API bool				  DeleteScene(std::string sceneId, std::string& projectDir,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		Tab*& openedTab,
		std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts);

	//getter for instance
	SCENEMANAGER_API static SceneManager& GetInstance();
private:
	//singleton
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
public:
	//variables
	std::map<std::string, std::string> scenes;
	std::map<std::string, std::unique_ptr<Scene>> openedScenes;
	Scene* currentScene = nullptr;
};

