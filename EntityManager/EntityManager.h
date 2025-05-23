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
#include "AssetSaver.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

//Store type of each light
enum LightType {
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT
};

//The light object
struct Light {
	LightType type;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	float cutOff = 0.0f;
	float outerCutOff = 0.0f;

	float constant = 0.0f;
	float linear = 0.0f;
	float quadratic = 0.0f;

	glm::mat4 lightSpaceMatrix;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	std::vector<glm::mat4> shadowTransforms;
	float far_plane = 0.0f;

	std::string id = "";
};

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
	ENTITYMANAGER_API void SetLights();
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

	//status
	ENTITYMANAGER_API size_t										  GetEntityCount() { return entities.size(); }
	ENTITYMANAGER_API std::map<std::string, std::shared_ptr<Entity>>& GetEntities() { return entities; }
	ENTITYMANAGER_API std::vector<Light>&							  GetLights() { return lights; }
private:
	//stores lights
	std::vector<Light> lights;

	//stores entities
	std::map<std::string, std::shared_ptr<Entity>> entities;

	//stores previous count of entities to check if an entity is added of removed
	size_t lastEntitySize = (size_t)0;
};