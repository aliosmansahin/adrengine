#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "AssetSaver.h"

/*
	All window classes will be included here
*/
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
#include "WindowTileMapEdit.h"
#include "WindowTileMapBrush.h"
#include "WindowTileMapViewer.h"
#include "WindowFlipBookEdit.h"
#include "WindowProjectDialog.h"
#include "WindowModalDialog.h"

class INTERFACEMANAGER_API MenuBar
{
public:
	//functions
	void			DrawMenuBar();
	static MenuBar& GetInstance();
private:
	//singleton
	MenuBar() = default;
	~MenuBar() = default;
	MenuBar(const MenuBar&) = delete;
	MenuBar& operator=(const MenuBar&) = delete;
};

