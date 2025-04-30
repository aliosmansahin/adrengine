#pragma once

#include "EntityManager.h"
#include "InputManager.h"

class EntityManager;

class Scene
{
public:
	bool CreateScene(std::string sceneId);
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
private:
	EntityManager* entityManager = nullptr;
	bool isDragging = false;
	bool skipThisFrame = false;
	int firstMouseX;
	int firstMouseY;
};

