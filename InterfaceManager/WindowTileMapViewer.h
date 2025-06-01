#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Localization.h"
#include "Logger.h"
#include "TileMap.h"

class WindowTileMapViewer
{
public:
	INTERFACEMANAGER_API void DrawWindow();

	//singleton
	INTERFACEMANAGER_API static WindowTileMapViewer& GetInstance();
private:
	WindowTileMapViewer() = default;
	~WindowTileMapViewer() = default;
	WindowTileMapViewer(const WindowTileMapViewer&) = delete;
	WindowTileMapViewer operator=(const WindowTileMapViewer&) = delete;
public:
	bool showWindow = false;
	TileMap* edittingTileMap = nullptr;
	int window_width = 0;
	int window_height = 0;

	//camera
	float cameraX;
	float cameraY;
};