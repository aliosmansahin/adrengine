#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "EntityManager.h"
#include "WindowEntityProperties.h"
#include "WindowAddEntity.h"

class WindowScene
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API void DrawEntity(std::shared_ptr<Entity>& entity, int depth = 0);
	INTERFACEMANAGER_API void CheckForChildren(bool& toChildren, Entity* entity, std::string droppedId);
	INTERFACEMANAGER_API static WindowScene& GetInstance();
private:
	//singleton
	WindowScene() = default;
	~WindowScene() = default;
	WindowScene(const WindowScene&) = delete;
	WindowScene& operator=(const WindowScene&) = delete;
private:
	//a vector to store entity changings
	std::vector<std::function<void()>> deferredEntityChanges;
public:
	//variables
	bool showWindow = true;
	bool focused = false;
	bool deletePressed = false;
	bool pendingDelete = false;
	std::string selectedId = "";
	std::shared_ptr<Entity> addParent = nullptr;
};