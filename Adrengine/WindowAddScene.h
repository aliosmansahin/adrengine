#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "SceneManager.h"
#include "WindowEntityProperties.h"
#include "WindowScene.h"

enum SceneType {
	SCENE_2D,
	SCENE_3D
};

class WindowAddScene
{
public:
	void DrawWindow();
	static WindowAddScene& GetInstance();
private:
	WindowAddScene() = default;
	~WindowAddScene() = default;
	WindowAddScene(const WindowAddScene&) = delete;
	WindowAddScene& operator=(const WindowAddScene&) = delete;
public:
	bool showWindow = false;
};

