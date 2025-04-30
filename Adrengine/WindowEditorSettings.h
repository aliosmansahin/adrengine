#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Localization.h"
#include "InterfaceManager.h"

class WindowEditorSettings
{
public:
	void DrawWindow();
	static WindowEditorSettings& GetInstance();
private:
	WindowEditorSettings() = default;
	~WindowEditorSettings() = default;
	WindowEditorSettings(const WindowEditorSettings&) = delete;
	WindowEditorSettings& operator=(const WindowEditorSettings&) = delete;
public:
	bool showWindow = false;
};