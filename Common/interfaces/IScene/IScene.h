#pragma once

#include "../IEntityManager/IEntityManager.h"

#include "../../utils/Utils.h"

#include "../IPhysics/IPhysics.h"

class IScene
{
public:
	//Getter for entity manager that belongs to the scene
	virtual std::shared_ptr<IEntityManager> GetEntityManager() = 0;

	//Handle scene
	virtual bool CreateScene(std::string sceneId, Utils::SceneType sceneType) = 0;
	virtual void DrawScene(int window_width, int window_height) = 0;
	virtual void UpdateScene(
		bool isPlaying,
		bool windowGameViewportIsHovered,
		bool windowGameViewportIsFocused,
		int window_width,
		int window_height,
		bool windowSceneFocused,
		bool windowSceneDeletePressed,
		bool& pendingDelete,
		std::string selectedId,
		std::function<void(std::string)> extraDeletingFunc
	) = 0;
	virtual void ReleaseScene() = 0;
	virtual nlohmann::json ToJson() = 0;
	virtual void FromJson(const nlohmann::json& json) = 0;

	//Getters
	virtual std::string GetSceneId() = 0;
	virtual std::string GetSceneName() = 0;
	virtual Utils::SceneType GetSceneType() = 0;
	virtual std::shared_ptr<ICamera> GetCurrentCamera() = 0;
	virtual std::shared_ptr<IPhysics> GetPhysics() = 0;
	virtual std::pair<float, float> GetDeltaXY() = 0;

	virtual bool GetLeftPressed() = 0;
	virtual bool GetDeletePressed() = 0;

	virtual ~IScene() = default;
};