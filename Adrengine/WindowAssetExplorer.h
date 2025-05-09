#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "AssetDatabase.h"
#include "WindowAddAsset.h"
#include "Engine.h"

class WindowAssetExplorer
{
public:
	void DrawWindow();
	static WindowAssetExplorer& GetInstance();
private:
	WindowAssetExplorer() = default;
	~WindowAssetExplorer() = default;
	WindowAssetExplorer(const WindowAssetExplorer&) = delete;
	WindowAssetExplorer& operator=(const WindowAssetExplorer&) = delete;
public:
	bool showWindow = true;
	std::string selectedId = "";
};

