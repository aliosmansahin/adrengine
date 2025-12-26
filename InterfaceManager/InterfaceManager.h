#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "glad_wrapper.h"
#include "glfw/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "imnodes/imnodes.h"

#include "Logger.h"

#include <thread>

#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"
#include "interfaces/ISceneManager/ISceneManager.h"
#include "interfaces/IEntity/IEntity.h"
#include "interfaces/IProject/IProject.h"

#include "ServiceLocator.h"

class InterfaceManager
{
public:
	//main funcs
	INTERFACEMANAGER_API bool InitInterface(GLFWwindow* window, ImGuiContext*& context, ImNodesContext*& nodesContext);
	INTERFACEMANAGER_API void CloseInterface();
	INTERFACEMANAGER_API void ResetInterface();
	//drawing
	INTERFACEMANAGER_API void StartFrame();
	INTERFACEMANAGER_API void EndFrame();
	INTERFACEMANAGER_API void DrawInterface();
	INTERFACEMANAGER_API void UpdateViewportContext();
	INTERFACEMANAGER_API void SetDarkTheme();
	INTERFACEMANAGER_API GLFWwindow* GetFocusedViewport();

	//Tabs
	INTERFACEMANAGER_API void AddTab(std::string tabId, Utils::TabType tabType);
	INTERFACEMANAGER_API void ActivateTab(std::string tabId);
	INTERFACEMANAGER_API void RemoveTab(std::string tabId);
	INTERFACEMANAGER_API void RemoveAllTabs();
	INTERFACEMANAGER_API int  GetTabHeight() const { return tabHeight; };

	//Getters for tabs
	INTERFACEMANAGER_API std::shared_ptr<Utils::Tab> GetTabById(std::string tabId);
	INTERFACEMANAGER_API std::shared_ptr<Utils::Tab> GetDeletingTab();
	INTERFACEMANAGER_API std::shared_ptr<Utils::Tab> GetOpenedTab() { return openedTab; }
	INTERFACEMANAGER_API std::string GetSelectedTabId() { return selectedTabId; }
	INTERFACEMANAGER_API std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& GetTabs() { return tabs; }

	//Pending tab delete
	INTERFACEMANAGER_API bool GetPendingTabDelete() { return pendingTabDelete; }
	INTERFACEMANAGER_API void SetPendingTabDelete(bool pending) { pendingTabDelete = pending; }

private:
	void DrawDockSpace();
	void DrawTabbar();
	void DrawWindows();

public:
	//getter for the instance
	INTERFACEMANAGER_API static InterfaceManager& GetInstance();

private:
	//singleton
	InterfaceManager() = default;
	~InterfaceManager() = default;
	InterfaceManager(const InterfaceManager&) = delete;
	InterfaceManager& operator=(const InterfaceManager&) = delete;

private:
	GLFWwindow* window = nullptr;
	bool darkTheme = true;
	bool pendingTabDelete = false;
	std::string deleteTabId = "";
	std::string selectedTabId = "";
	std::shared_ptr<Utils::Tab> openedTab = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Utils::Tab>> tabs;
	int tabHeight = 40;
};