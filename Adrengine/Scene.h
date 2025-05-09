#pragma once

#include "EntityManager.h"
#include "InputManager.h"

class EntityManager;
enum SceneType;

class Scene
{
public:
	bool CreateScene(std::string sceneId, SceneType sceneType);
	void DrawScene();
	void UpdateScene();
	void ReleaseScene();
	EntityManager* GetEntityManager() { return entityManager; }
	nlohmann::json ToJson();
	void FromJson(const nlohmann::json& json);
public:
	std::string sceneId;
	std::string sceneName;
	float cameraX = 0;
	float cameraY = 0;
	float cameraZ = 0;
	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, -1.0f);
	SceneType sceneType;
private:
	EntityManager* entityManager = nullptr;
	bool isDragging = false;
	bool skipThisFrame = false;
	int firstMouseX = 0;
	int firstMouseY = 0;
};

