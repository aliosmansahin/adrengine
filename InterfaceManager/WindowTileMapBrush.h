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
	void DrawWindow();
	static WindowTileMapBrush& GetInstance();
private:
	WindowTileMapBrush() = default;
	~WindowTileMapBrush() = default;
	WindowTileMapBrush(const WindowTileMapBrush&) = delete;
	WindowTileMapBrush& operator=(const WindowTileMapBrush&) = delete;
public:
	bool showWindow = false;
	TileMap* editingTileMap = nullptr;
	bool editing = false;
	std::pair<int, int> selectedTile = { -1, -1 };
};

