#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imnodes/imnodes.h"

#include "Logger.h"

#include "MenuBar.h"
#include "SceneManager.h"

enum TabType {
	SceneEditor,
	VisualScriptEditor
};

struct Tab {
	std::string id;
	TabType tabType;
};

class InterfaceManager
{
public:
	//main funcs
	bool InitInterface(GLFWwindow* window);
	void CloseInterface();
	//drawing
	void StartFrame();
	void EndFrame();
	void DrawInterface();
	void DrawDockSpace();
	void UpdateViewportContext();
	void SetDarkTheme();
public:
	//getter for the instance
	static InterfaceManager& GetInstance();
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
	std::unordered_map<std::string, std::unique_ptr<Tab>> tabs;
	int tabHeight = 40;
private:
};