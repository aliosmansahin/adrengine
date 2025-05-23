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
#include "AssetSaver.h"

class INTERFACEMANAGER_API MenuBar
{
public:
	//functions
	void			DrawMenuBar(std::function<void()>& saveFunc);
	static MenuBar& GetInstance();
private:
	//singleton
	MenuBar() = default;
	~MenuBar() = default;
	MenuBar(const MenuBar&) = delete;
	MenuBar& operator=(const MenuBar&) = delete;
};

