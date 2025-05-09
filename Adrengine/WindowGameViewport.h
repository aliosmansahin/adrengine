#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "EntityManager.h"
#include "Graphics.h"

class WindowGameViewport
{
public:
	void DrawWindow();

	static WindowGameViewport& GetInstance();
private:
	WindowGameViewport() = default;
	~WindowGameViewport() = default;
	WindowGameViewport(const WindowGameViewport&) = delete;
	WindowGameViewport& operator=(const WindowGameViewport&) = delete;
public:
	bool showWindow = true;
	bool isPlaying = false;
	bool isFocused = false;
	bool isHovered = false;
	float window_width = 0;
	float window_height = 0;
};

