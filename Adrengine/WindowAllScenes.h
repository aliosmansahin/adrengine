#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "SceneManager.h"

class WindowAllScenes
{
public:
	void DrawWindow();
	static WindowAllScenes& GetInstance();
private:
	WindowAllScenes() = default;
	~WindowAllScenes() = default;
	WindowAllScenes(const WindowAllScenes&);
	WindowAllScenes& operator=(const WindowAllScenes&) = delete;
public:
	bool showWindow = true;
	std::string selectedSceneId = "";
	bool pendingDelete = false;
};

