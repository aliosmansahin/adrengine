#pragma once

#ifdef SCENEMANAGER_EXPORTS 
#define SCENEMANAGER_API __declspec(dllexport)
#else
#define SCENEMANAGER_API __declspec(dllimport)
#endif

#include "Scene.h"

#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"
#include "interfaces/ISceneManager/ISceneManager.h"
#include "interfaces/IProject/IProject.h"

#include "ServiceLocator.h"

class SceneManager : public ISceneManager
{
public:
	//manager functions
	SCENEMANAGER_API bool				  InitializeManager() override;
	SCENEMANAGER_API void				  ClearManager() override;

	//scene functions
	SCENEMANAGER_API std::shared_ptr<IScene> CreateScene(Utils::SceneType sceneType) override;

	SCENEMANAGER_API std::shared_ptr<IScene> LoadScene(std::string sceneId) override;

	SCENEMANAGER_API bool				  CloseScene(std::string sceneId) override;
	SCENEMANAGER_API bool				  DeleteScene(std::string sceneId) override;

	//Getters
	SCENEMANAGER_API std::map<std::string, std::string>& GetScenes() override;
	SCENEMANAGER_API std::shared_ptr<IScene> GetOpenedScene() override;

public:
	//Singleton
	static SceneManager& GetInstance() {
		static SceneManager instance;
		return instance;
	}

public:
	//singleton
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

private:
	//variables
	std::map<std::string, std::string> scenes;
	std::shared_ptr<IScene> openedScene;
};

