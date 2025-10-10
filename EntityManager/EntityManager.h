#pragma once

//STL
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//Entities
#include "Entity.h"
#include "Sprite2D.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "TileMap.h"
#include "FlipBook.h"
#include "Camera.h"

#include "ShaderManager.h"

//Glad
#include "glad_wrapper.h"

//Other headers that are needed
#include "AssetSaver.h"

//GLM
#include <glm/gtx/euler_angles.hpp>

//Includes for interfacec
#include "interfaces/IEntityManager/IEntityManager.h"
#include "interfaces/ISceneManager/ISceneManager.h"
#include "interfaces/IProject/IProject.h"
#include "interfaces/IEngine/IEngine.h"

#include "ServiceLocator.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

using namespace adr;

class EntityManager : public IEntityManager
{
public:
	//main funcs
	ENTITYMANAGER_API bool InitEntityManager() override;
	ENTITYMANAGER_API void DrawEntities(int window_width, int window_height, glm::vec3 currentSceneCameraPos, bool is3D) override;
	ENTITYMANAGER_API void UpdateEntities(
		bool windowSceneFocused,
		bool windowSceneDeletePressed,
		bool& pendingDelete,
		bool isPlaying,
		std::string selectedId,
		std::function<void(std::string)> extraDeletingFunc,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		std::shared_ptr<ICamera>& gameCamera
		) override;
	ENTITYMANAGER_API void ReleaseEntityManager() override;

	//management for entities
	ENTITYMANAGER_API std::string CreateEntity(
		std::string type,
		std::string sceneId,
		nlohmann::json& currentSceneJson,
		std::shared_ptr<IEntity>& parent
	) override;
	ENTITYMANAGER_API bool		  RemoveEntity(
		std::shared_ptr<IEntity> entity,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		bool saveScene = true) override;

	ENTITYMANAGER_API void		  LoadEntitiesFromJson(
		const nlohmann::json& json,
		std::shared_ptr<IScene> scene) override;

	ENTITYMANAGER_API void		  BuildEntityHierarchy() override;

	ENTITYMANAGER_API void		  RunEntitiesScriptBegin() override;
	ENTITYMANAGER_API void		  ResetEntitiesRuntimeValues() override;
	ENTITYMANAGER_API void        SetRigitbodiesFromEntities() override;
	ENTITYMANAGER_API void        SetEntitiesFromRigidbodies() override;

	//status
	ENTITYMANAGER_API size_t										  GetEntityCount() override { return entities.size(); }

	//getters
	ENTITYMANAGER_API std::map<std::string, std::shared_ptr<IEntity>>& GetEntities() override { return entities; }
	ENTITYMANAGER_API std::shared_ptr<IEntity> GetEntityById(std::string id) override;

private:
	//helpers
	void PerformEntityDeletions(
		bool windowSceneFocused,
		bool windowSceneDeletePressed,
		bool& pendingDelete,
		std::string selectedId,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		std::function<void(std::string)> extraDeletingFunc);
	void SetEntityRealStats(std::shared_ptr<IEntity> entity);

private:
	//stores entities
	std::map<std::string, std::shared_ptr<IEntity>> entities;

	//stores previous count of entities to check if an entity is added of removed
	size_t lastEntitySize = (size_t)0;
};