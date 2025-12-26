#pragma once

#include <memory>
#include <map>
#include <string>

#include "../IScene/IScene.h"
#include "../IPhysics/IPhysics.h"

class ISceneManager
{
public:
	virtual ~ISceneManager() = default;

	//main functions
	virtual bool InitializeManager() = 0;
	virtual void ClearManager() = 0;

	//Handle scenes
	virtual std::shared_ptr<IScene> CreateScene(Utils::SceneType sceneType) = 0;
	virtual std::shared_ptr<IScene> LoadScene(std::string sceneId) = 0;
	virtual bool CloseScene(std::string sceneId) = 0;
	virtual bool DeleteScene(std::string sceneId) = 0;

	virtual std::map<std::string, std::string>& GetScenes() = 0;
	virtual std::shared_ptr<IScene> GetOpenedScene() = 0;
};

