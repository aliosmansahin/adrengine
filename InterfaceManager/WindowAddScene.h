#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include <unordered_map>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "utils/Utils.h"

#include "ServiceLocator.h"

class WindowAddScene
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowAddScene& GetInstance();
	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_add_scene";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Add Scene";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	WindowAddScene() = default;
	~WindowAddScene() = default;
	WindowAddScene(const WindowAddScene&) = delete;
	WindowAddScene& operator=(const WindowAddScene&) = delete;
public:
	//variables
	bool showWindow = false;

private:
	ImVec2 defSize{ 400.0f, 400.0f };
};

