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

#include "interfaces/IEntity/ISprite2D/ITileMap/ITileMap.h"

class WindowTileMapBrush
{
public:
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowTileMapBrush& GetInstance();
	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_tilemap_brush";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Tile Map Brush";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	INTERFACEMANAGER_API WindowTileMapBrush() = default;
	INTERFACEMANAGER_API ~WindowTileMapBrush() = default;
	INTERFACEMANAGER_API WindowTileMapBrush(const WindowTileMapBrush&) = delete;
	INTERFACEMANAGER_API WindowTileMapBrush& operator=(const WindowTileMapBrush&) = delete;
public:
	bool showWindow = false;
	std::shared_ptr<ITileMap> editingTileMap = nullptr;
	bool editing = false;
	std::pair<int, int> selectedTile = { -1, -1 };

private:
	ImVec2 defSize{ 400.0f, 400.0f };
};

