#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include <memory>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "EntityManager.h"

class WindowEntityProperties
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(
		std::string& projectDir,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		Tab*& openedTab,
		std::string& selectedTabId
	);
	INTERFACEMANAGER_API void SelectEntity(Entity* entity);
	INTERFACEMANAGER_API static WindowEntityProperties& GetInstance();
private:
	//singleton
	WindowEntityProperties() = default;
	~WindowEntityProperties() = default;
	WindowEntityProperties(const WindowEntityProperties&) = delete;
	WindowEntityProperties& operator=(const WindowEntityProperties&) = delete;
public:
	//variables
	bool showWindow = true;
	Entity* currentEntity;
};