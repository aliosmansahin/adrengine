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
#include "AssetSaver.h"

#include "glad_wrapper.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

using namespace adr;

class EntityManager
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
		std::function<void()> selectFunction,
		std::string& projectDir,
		std::string& sceneId,
		nlohmann::json& currentSceneJson
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
		std::string which,
		std::string& projectDir,
		std::string& sceneId,
		nlohmann::json& currentSceneJson,
		bool saveScene = true);

	ENTITYMANAGER_API void		  SetEntityRealStats(Entity* entity);

	//status
	ENTITYMANAGER_API size_t										  GetEntityCount() { return entities.size(); }
	ENTITYMANAGER_API std::map<std::string, std::shared_ptr<Entity>>& GetEntities() { return entities; }
private:
	//stores lights

	//stores entities
	std::map<std::string, std::shared_ptr<Entity>> entities;

	//stores previous count of entities to check if an entity is added of removed
	size_t lastEntitySize = (size_t)0;
};