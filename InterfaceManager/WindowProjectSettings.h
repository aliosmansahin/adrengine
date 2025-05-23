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

class WindowProjectSettings
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowProjectSettings& GetInstance();
private:
	//singleton
	WindowProjectSettings() = default;
	~WindowProjectSettings() = default;
	WindowProjectSettings(const WindowProjectSettings&) = delete;
	WindowProjectSettings& operator=(const WindowProjectSettings&) = delete;
public:
	//variables
	bool showWindow = false;
};