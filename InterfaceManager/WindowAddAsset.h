#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imguifiledialog/ImGuiFileDialog.h"

#include "Logger.h"

#include "interfaces/IProject/IProject.h"

#include "ServiceLocator.h"

class WindowAddAsset
{
public:
	//main functions
	INTERFACEMANAGER_API void					DrawWindow(std::string& assetExplorerType);
	INTERFACEMANAGER_API static WindowAddAsset& GetInstance();
	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_add_asset";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Add New Asset";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	WindowAddAsset() = default;
	~WindowAddAsset() = default;
	WindowAddAsset(const WindowAddAsset&) = delete;
	WindowAddAsset& operator=(const WindowAddAsset&) = delete;
public:
	//variables
	bool showWindow = false;
	char buf[256];

private:
	ImVec2 defSize{ 700.0f, 400.0f };
};

