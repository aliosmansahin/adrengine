#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "SceneManager.h"

class WindowAddEntity
{
public:
	void DrawWindow();
	static WindowAddEntity& GetInstance();
private:
	WindowAddEntity() = default;
	~WindowAddEntity() = default;
	WindowAddEntity(const WindowAddEntity&) = delete;
	WindowAddEntity& operator=(const WindowAddEntity&) = delete;
public:
	bool showWindow = false;
	bool shouldFocus = true;
	std::vector<std::string> types;
	char typeBuf[32] = {};
};