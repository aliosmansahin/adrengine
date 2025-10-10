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
#include "Graphics.h"

#include "glad_wrapper.h"

#include "interfaces/IEntity/IObject/IObject.h"
#include "interfaces/IEntity/ISprite2D/ITileMap/ITileMap.h"
#include "interfaces/ISceneManager/ISceneManager.h"

#include "interfaces/IEngine/IEngine.h"

#include "ServiceLocator.h"

class WindowGameViewport
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();

	INTERFACEMANAGER_API static WindowGameViewport& GetInstance();
private:
	//singleton
	WindowGameViewport() = default;
	~WindowGameViewport() = default;
	WindowGameViewport(const WindowGameViewport&) = delete;
	WindowGameViewport& operator=(const WindowGameViewport&) = delete;
public:
	//variables
	bool showWindow = true;
	bool isPlaying = false;
	bool isFocused = false;
	bool isHovered = false;
	float window_width = 800;
	float window_height = 600;
	int mouseX;
	int mouseY;
};

