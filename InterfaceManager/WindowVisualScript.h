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

#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"
#include "interfaces/IVisualScript/INode/INode.h"
#include "interfaces/ISceneManager/ISceneManager.h"

#include "interfaces/IVisualScript/INode/IGetThisEntity/IGetThisEntity.h"

#include "utils/Utils.h"

#include "ServiceLocator.h"

class WindowVisualScript
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowVisualScript& GetInstance();
	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_visual_script";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Visual Script";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
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