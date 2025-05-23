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
#include "Print.h"
#include "Begin.h"
#include "Return.h"
#include "VisualScriptManager.h"

class WindowVisualScript
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(
		int tabHeight,
		std::string& projectDir,
		Tab*& openedTab,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs
		);
	INTERFACEMANAGER_API static WindowVisualScript& GetInstance();
private:
	//singleton
	WindowVisualScript() = default;
	~WindowVisualScript() = default;
	WindowVisualScript(const WindowVisualScript&) = delete;
	WindowVisualScript& operator=(const WindowVisualScript&) = delete;
public:
	//variables
	bool showWindow = true;
};