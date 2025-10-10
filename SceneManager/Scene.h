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

#include "ShaderManager.h"

#include "utils/Utils.h"

#include "interfaces/IEntityManager/IEntityManager.h"
#include "Timer.h"
#include "InputManager.h"

#include "interfaces/IScene/IScene.h"
#include "interfaces/IEntity/ICamera/ICamera.h"
#include "interfaces/IEngine/IEngine.h"
#include "interfaces/IProject/IProject.h"

class Scene : public IScene, public std::enable_shared_from_this<Scene>
{
public:
	//main functions
	SCENEMANAGER_API bool			CreateScene(std::string sceneId, Utils::SceneType sceneType) override;
	SCENEMANAGER_API void			DrawScene(int window_width, int window_height) override;
	SCENEMANAGER_API void			UpdateScene(
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
	) override;
	SCENEMANAGER_API void			ReleaseScene() override;
	SCENEMANAGER_API std::shared_ptr<IEntityManager> GetEntityManager() override { return entityManager; }

	//json functions
	SCENEMANAGER_API nlohmann::json ToJson() override;
	SCENEMANAGER_API void			FromJson(const nlohmann::json& json) override;

	//Getters
	SCENEMANAGER_API std::string GetSceneId() override;
	SCENEMANAGER_API std::string GetSceneName() override;
	SCENEMANAGER_API Utils::SceneType GetSceneType() override;
	SCENEMANAGER_API std::shared_ptr<ICamera> GetCurrentCamera() override;
	SCENEMANAGER_API std::shared_ptr<IPhysics> GetPhysics() override;

	SCENEMANAGER_API std::pair<float, float> GetDeltaXY() override;

	SCENEMANAGER_API bool GetLeftPressed() override;
	SCENEMANAGER_API bool GetDeletePressed() override;
private:
	//helpers
	void UpdateEditorCamera(
		int currentMouseX,
		int currentMouseY,
		int window_width,
		int window_height,
		bool windowGameViewportIsFocused,
		bool isPlaying);
	void UpdateTransformMatrixForTheCamera(int window_width, int window_height);

private:
	//scene variables
	std::string sceneId;
	std::string sceneName;
	Utils::SceneType sceneType;

	//camera
	std::shared_ptr<ICamera> editorCamera = nullptr;
	std::shared_ptr<ICamera> gameCamera = nullptr;
	std::shared_ptr<ICamera> currentCamera = nullptr;

	//Delta mouse position
	float deltaX;
	float deltaY;

	//Buttons and keys
	bool leftPressed = false;
	bool deletePressed = false;

	//This is the script of the scene
	std::shared_ptr<IVisualScript> sceneScript; //TODO: Add a getter

	//physics
	std::shared_ptr<IPhysics> physics = nullptr;

private:
	//entity manager
	std::shared_ptr<IEntityManager> entityManager = nullptr;
	
	//dragging
	bool isDragging = false;
	bool skipThisFrame = false;
	int firstMouseX = 0;
	int firstMouseY = 0;
};

