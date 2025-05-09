#pragma once

#include "Scene.h"
#include "InterfaceManager.h"

class Scene;
enum SceneType;

class SceneManager
{
public:
	bool InitializeManager();
	void ClearManager();
	bool CreateScene(SceneType sceneType);
	Scene* LoadScene(std::string sceneId);
	bool CloseScene(std::string sceneId);
	bool DeleteScene(std::string sceneId);
	static SceneManager& GetInstance();
private:
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
public:
	std::map<std::string, std::string> scenes;
	std::map<std::string, std::unique_ptr<Scene>> openedScenes;
	Scene* currentScene = nullptr;
};

