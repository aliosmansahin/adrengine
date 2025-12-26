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

#include "AssetDatabase.h"

class WindowTileMapEdit
{
public:
	//main functions
	INTERFACEMANAGER_API void						DrawWindow();
	INTERFACEMANAGER_API static WindowTileMapEdit& GetInstance();
	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_tilemap_edit";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Edit Tile Map";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	INTERFACEMANAGER_API WindowTileMapEdit() = default;
	INTERFACEMANAGER_API ~WindowTileMapEdit() = default;
	INTERFACEMANAGER_API WindowTileMapEdit(const WindowTileMapEdit&) = delete;
	INTERFACEMANAGER_API WindowTileMapEdit& operator=(const WindowTileMapEdit&) = delete;
public:
	bool showWindow = true;
	std::shared_ptr<ITileMap> editingTileMap = nullptr;

private:
	ImVec2 defSize{ 600.0f, 600.0f };
};