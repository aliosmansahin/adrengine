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

#include <unordered_map>

#include "utils/Utils.h"

#include "interfaces/ISceneManager/ISceneManager.h"

class WindowAllScenes
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowAllScenes& GetInstance();
private:
	//singleton
	WindowAllScenes() = default;
	~WindowAllScenes() = default;
	WindowAllScenes(const WindowAllScenes&);
	WindowAllScenes& operator=(const WindowAllScenes&) = delete;
public:
	//variables
	bool showWindow = true;
	std::string selectedSceneId = "";
	bool pendingDelete = false;
};

