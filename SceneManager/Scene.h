#pragma once

#ifdef SCENEMANAGER_EXPORTS
#define SCENEMANAGER_API __declspec(dllexport)
#else
#define SCENEMANAGER_API __declspec(dllimport)
#endif

#include <string>
#include <unordered_map>
#include <map>
#include <memory>

#include "Entity.h"

class EntityManager;

//enum for scene types
enum SceneType {
	SCENE_2D,
	SCENE_3D
};

class Scene
{
public:
	//main functions
	SCENEMANAGER_API bool			CreateScene(std::string sceneId, SceneType sceneType);
	SCENEMANAGER_API void			DrawScene(int window_width, int window_height, glm::vec3 currentSceneCameraPos);
	SCENEMANAGER_API void			UpdateScene(
		bool isPlaying,
		bool windowGameViewportIsHovered,
		int screenWidth,
		int screenHeight,
		int window_width,
		int window_height,
		bool windowSceneFocused,
		bool windowSceneDeletePressed,
		bool& pendingDelete,
		std::string selectedId,
		std::function<void()> selectFunction,
		std::string& projectDir
	);
	SCENEMANAGER_API void			ReleaseScene();
	SCENEMANAGER_API EntityManager* GetEntityManager() { return entityManager; }

	//json functions
	SCENEMANAGER_API nlohmann::json ToJson();
	SCENEMANAGER_API void			FromJson(const nlohmann::json& json, std::string projectDir, std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes);
public:
	//scene variables
	std::string sceneId;
	std::string sceneName;
	SceneType sceneType;

	//camera
	float cameraX = 0;
	float cameraY = 0;
	float cameraZ = 0;
	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, -1.0f);
private:
	//entity manager
	EntityManager* entityManager = nullptr;
	
	//dragging
	bool isDragging = false;
	bool skipThisFrame = false;
	int firstMouseX = 0;
	int firstMouseY = 0;
};

