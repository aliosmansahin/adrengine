#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "SceneManager.h"

class WindowAddAsset
{
public:
	void DrawWindow();
	static WindowAddAsset& GetInstance();
private:
	WindowAddAsset() = default;
	~WindowAddAsset() = default;
	WindowAddAsset(const WindowAddAsset&) = delete;
	WindowAddAsset& operator=(const WindowAddAsset&) = delete;
public:
	bool showWindow = false;
	char buf[256];
};

