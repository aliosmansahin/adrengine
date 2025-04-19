#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "WindowProjectSettings.h"

class MenuBar
{
public:
	void DrawMenuBar();
	static MenuBar& GetInstance();
private:
	MenuBar() = default;
	~MenuBar() = default;
	MenuBar(const MenuBar&) = delete;
	MenuBar& operator=(const MenuBar&) = delete;
};

