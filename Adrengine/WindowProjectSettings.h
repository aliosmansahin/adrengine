#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

class WindowProjectSettings
{
public:
	void DrawWindow();
	static WindowProjectSettings& GetInstance();
private:
	WindowProjectSettings() = default;
	~WindowProjectSettings() = default;
	WindowProjectSettings(const WindowProjectSettings&) = delete;
	WindowProjectSettings& operator=(const WindowProjectSettings&) = delete;
public:
	bool showWindow = false;
};