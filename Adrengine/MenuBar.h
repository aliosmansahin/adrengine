#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "WindowProjectSettings.h"
#include "WindowEditorSettings.h"
#include "WindowScene.h"
#include "WindowGameViewport.h"
#include "WindowEntityProperties.h"
#include "WindowAddEntity.h"
#include "WindowAddAsset.h"
#include "WindowAddScene.h"
#include "WindowAllScenes.h"
#include "WindowVisualScript.h"
#include "WindowAssetExplorer.h"
#include "FileIO.h"
#include "AssetSaver.h"

class MenuBar
{
public:
	void DrawMenuBar();
	static MenuBar& GetInstance();
private:
	MenuBar() = default;
	~MenuBar() = default;
	MenuBar(const MenuBar&) = delete;
	MenuBar& operator=(const MenuBar&) = delete;
};

