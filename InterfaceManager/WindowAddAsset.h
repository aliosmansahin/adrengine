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

class WindowAddAsset
{
public:
	//main functions
	INTERFACEMANAGER_API void					DrawWindow(std::string& projectDir, std::string& assetExplorerType);
	INTERFACEMANAGER_API static WindowAddAsset& GetInstance();
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
};

