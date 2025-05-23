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
#include "EntityManager.h"
#include "Graphics.h"

class WindowGameViewport
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(
		float engineFPS,
		float engineMS
	);

	INTERFACEMANAGER_API static WindowGameViewport& GetInstance();
private:
	//singleton
	WindowGameViewport() = default;
	~WindowGameViewport() = default;
	WindowGameViewport(const WindowGameViewport&) = delete;
	WindowGameViewport& operator=(const WindowGameViewport&) = delete;
public:
	//variables
	bool showWindow = true;
	bool isPlaying = false;
	bool isFocused = false;
	bool isHovered = false;
	float window_width = 0;
	float window_height = 0;
};

