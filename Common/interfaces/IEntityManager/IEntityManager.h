#pragma once

#include "../IEntity/IEntity.h"
#include "../IEntity/ICamera/ICamera.h"

#include <memory>
#include <string>

class Physics;

class IEntityManager
{
public:
	//layer functions
	virtual bool InitEntityManager() = 0;
	virtual void DrawEntities(int window_width, int window_height, glm::vec3 currentSceneCameraPos, bool is3D) = 0;
	virtual void UpdateEntities(
		bool windowSceneFocused,
		bool windowSceneDeletePressed,
		bool& pendingDelete,
		bool isPlaying,
		std::string selectedId,
		std::function<void(std::string)> extraDeletingFunc,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		std::shared_ptr<ICamera>& gameCamera
	) = 0;
	virtual void ReleaseEntityManager() = 0;

	//Handle entities
	virtual std::string CreateEntity(
		std::string type,
		std::string sceneId,
		nlohmann::json& currentSceneJson,
		std::shared_ptr<IEntity>& parent
	) = 0;

	virtual bool RemoveEntity(
		std::shared_ptr<IEntity> entity,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		bool saveScene = true) = 0;

	virtual std::map<std::string, std::shared_ptr<IEntity>>& GetEntities() = 0;
	virtual size_t GetEntityCount() = 0;
	virtual std::shared_ptr<IEntity> GetEntityById(std::string id) = 0;
	virtual void BuildEntityHierarchy() = 0;

	virtual void LoadEntitiesFromJson(
		const nlohmann::json& json,
		std::shared_ptr<IScene> scene) = 0;

	//Scripts
	virtual void RunEntitiesScriptBegin() = 0;

	//Reset to editor values
	virtual void ResetEntitiesRuntimeValues() = 0;

	//Physics
	virtual void SetRigitbodiesFromEntities() = 0;
	virtual void SetEntitiesFromRigidbodies() = 0;

	virtual ~IEntityManager() = default;
};