#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Entity.h"
#include "Sprite2D.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "TileMap.h"
#include "FlipBook.h"
#include "Camera.h"
#include "AssetSaver.h"

#include "glad_wrapper.h"

#include "interfaces/IEntityManager/IEntityManager.h"

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
	ENTITYMANAGER_API bool InitEntityManager();
	ENTITYMANAGER_API void DrawEntities(int window_width, int window_height, glm::vec3 currentSceneCameraPos, bool is3D);
	ENTITYMANAGER_API void UpdateEntities(
		bool windowSceneFocused,
		bool windowSceneDeletePressed,
		bool& pendingDelete,
		std::string selectedId,
		std::function<void(std::string)> extraDeletingFunc,
		std::string& projectDir,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		Camera*& gameCamera
		);
	ENTITYMANAGER_API void ReleaseEntityManager();

	//management for entities
	ENTITYMANAGER_API std::string CreateEntity(
		std::string type,
		std::string& projectDir,
		std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
		std::string sceneId,
		nlohmann::json& currentSceneJson,
		std::shared_ptr<Entity>& parent,
		bool newEntity = true
	);
	ENTITYMANAGER_API bool		  RemoveEntity(
		Entity* entity,
		std::string& projectDir,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		bool saveScene = true);

	ENTITYMANAGER_API void		  SetEntityRealStats(Entity* entity);

	ENTITYMANAGER_API void		  RunEntitiesScriptBegin();

	//status
	ENTITYMANAGER_API size_t										  GetEntityCount() { return entities.size(); }

	//getters
	ENTITYMANAGER_API std::map<std::string, std::shared_ptr<Entity>>& GetEntities() { return entities; }
	ENTITYMANAGER_API Entity* GetEntityById(std::string id) override;
private:
	//stores entities
	std::map<std::string, std::shared_ptr<Entity>> entities;

	//stores previous count of entities to check if an entity is added of removed
	size_t lastEntitySize = (size_t)0;
};