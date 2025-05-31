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
#include "TileMap.h"

class WindowTileMapBrush
{
public:
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowTileMapBrush& GetInstance();
private:
	INTERFACEMANAGER_API WindowTileMapBrush() = default;
	INTERFACEMANAGER_API ~WindowTileMapBrush() = default;
	INTERFACEMANAGER_API WindowTileMapBrush(const WindowTileMapBrush&) = delete;
	INTERFACEMANAGER_API WindowTileMapBrush& operator=(const WindowTileMapBrush&) = delete;
public:
	bool showWindow = false;
	TileMap* editingTileMap = nullptr;
	bool editing = false;
	std::pair<int, int> selectedTile = { -1, -1 };
};

