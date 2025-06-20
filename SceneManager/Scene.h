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

#include "utils/Utils.h"

#include "EntityManager.h"
#include "Timer.h"
#include "InputManager.h"

#include "interfaces/IScene/IScene.h"

class Scene : public IScene
{
public:
	//main functions
	SCENEMANAGER_API bool			CreateScene(std::string sceneId, Utils::SceneType sceneType);
	SCENEMANAGER_API void			DrawScene(int window_width, int window_height);
	SCENEMANAGER_API void			UpdateScene(
		bool isPlaying,
		bool windowGameViewportIsHovered,
		bool windowGameViewportIsFocused,
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
	SCENEMANAGER_API EntityManager* GetEntityManager() override { return entityManager; }

	//json functions
	SCENEMANAGER_API nlohmann::json ToJson();
	SCENEMANAGER_API void			FromJson(const nlohmann::json& json, std::string projectDir, std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes);
public:
	//scene variables
	std::string sceneId;
	std::string sceneName;
	Utils::SceneType sceneType;

	//camera
	Camera* editorCamera = nullptr;
	Camera* gameCamera = nullptr;
	Camera* currentCamera = nullptr;

	//Delta mouse position
	float deltaX;
	float deltaY;

	//Buttons and keys
	bool leftPressed = false;
	bool deletePressed = false;

	//This is the script of the scene
	std::shared_ptr<VisualScript> sceneScript;
private:
	//entity manager
	EntityManager* entityManager = nullptr;
	
	//dragging
	bool isDragging = false;
	bool skipThisFrame = false;
	int firstMouseX = 0;
	int firstMouseY = 0;
};

