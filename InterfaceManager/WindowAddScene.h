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
#include "WindowEntityProperties.h"
#include "WindowScene.h"

class WindowAddScene
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(
		std::string& projectDir,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		Tab*& openedTab,
		std::string& selectedTabId
	);
	INTERFACEMANAGER_API static WindowAddScene& GetInstance();
private:
	//singleton
	WindowAddScene() = default;
	~WindowAddScene() = default;
	WindowAddScene(const WindowAddScene&) = delete;
	WindowAddScene& operator=(const WindowAddScene&) = delete;
public:
	//variables
	bool showWindow = false;
};

