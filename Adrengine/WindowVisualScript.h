#pragma once

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
	void DrawWindow();
	static WindowVisualScript& GetInstance();
private:
	WindowVisualScript() = default;
	~WindowVisualScript() = default;
	WindowVisualScript(const WindowVisualScript&) = delete;
	WindowVisualScript& operator=(const WindowVisualScript&) = delete;
public:
	bool showWindow = true;
};