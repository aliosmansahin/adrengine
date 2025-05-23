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
#include "SceneManager.h"

class WindowAllScenes
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(
		std::string& projectDir,
		std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
		bool& windowAddSceneShowWindow,
		Entity* windowEntityPropertiesCurrentEntity,
		std::string& windowSceneSelectedId,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		std::string& selectedTabId,
		Tab*& openedTab
	);
	INTERFACEMANAGER_API static WindowAllScenes& GetInstance();
private:
	//singleton
	WindowAllScenes() = default;
	~WindowAllScenes() = default;
	WindowAllScenes(const WindowAllScenes&);
	WindowAllScenes& operator=(const WindowAllScenes&) = delete;
public:
	//variables
	bool showWindow = true;
	std::string selectedSceneId = "";
	bool pendingDelete = false;
};

