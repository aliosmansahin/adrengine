#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include <functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Localization.h"

class WindowEditorSettings
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(bool* darkTheme, std::function<void()> setDarkThemeFunc);
	INTERFACEMANAGER_API static WindowEditorSettings& GetInstance();
private:
	//singleton
	WindowEditorSettings() = default;
	~WindowEditorSettings() = default;
	WindowEditorSettings(const WindowEditorSettings&) = delete;
	WindowEditorSettings& operator=(const WindowEditorSettings&) = delete;
public:
	//variables
	bool showWindow = false;
};