#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imnodes/imnodes.h"

#include "Logger.h"
#include "MenuBar.h"

class InterfaceManager
{
public:
	//main funcs
	INTERFACEMANAGER_API bool InitInterface(GLFWwindow* window);
	INTERFACEMANAGER_API void CloseInterface();
	//drawing
	INTERFACEMANAGER_API void StartFrame();
	INTERFACEMANAGER_API void EndFrame();
	INTERFACEMANAGER_API void DrawInterface(
		std::string& projectDir,
		std::function<void()> saveFunc,
		std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
		float engineFPS,
		float engineMS);
	INTERFACEMANAGER_API void DrawDockSpace();
	INTERFACEMANAGER_API void UpdateViewportContext();
	INTERFACEMANAGER_API void SetDarkTheme();
public:
	//getter for the instance
	INTERFACEMANAGER_API static InterfaceManager& GetInstance();
private:
	//singleton
	InterfaceManager() = default;
	~InterfaceManager() = default;
	InterfaceManager(const InterfaceManager&) = delete;
	InterfaceManager& operator=(const InterfaceManager&) = delete;
public:
	bool darkTheme = true;
	bool pendingTabDelete = false;
	std::string deleteTabId = "";
	std::string selectedTabId = "";
	Tab* openedTab = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Tab>> tabs;
	int tabHeight = 40;
};