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

class WindowTileMapEdit
{
public:
	//main functions
	void						DrawWindow();
	static WindowTileMapEdit& GetInstance();
private:
	//singleton
	WindowTileMapEdit() = default;
	~WindowTileMapEdit() = default;
	WindowTileMapEdit(const WindowTileMapEdit&) = delete;
	WindowTileMapEdit& operator=(const WindowTileMapEdit&) = delete;
public:
	bool showWindow = true;
	TileMap* editingTileMap = nullptr;
};