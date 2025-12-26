#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "AssetDatabase.h"

class WindowAssetExplorer
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API void DrawTexturesPage();
	INTERFACEMANAGER_API void DrawMeshesPage();
	INTERFACEMANAGER_API static WindowAssetExplorer& GetInstance();

	INTERFACEMANAGER_API std::string				 GetWindowId() {
		return "window_asset_explorer";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Asset Explorer";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}

private:
	//singleton
	WindowAssetExplorer() = default;
	~WindowAssetExplorer() = default;
	WindowAssetExplorer(const WindowAssetExplorer&) = delete;
	WindowAssetExplorer& operator=(const WindowAssetExplorer&) = delete;
public:
	//variables
	bool showWindow = true;
	std::string selectedId = "";
	std::string currentSelection = "Textures";
};

