#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include <functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Localization.h"

class WindowEditorSettings
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow(bool* darkTheme, std::function<void()> setDarkThemeFunc);
	INTERFACEMANAGER_API static WindowEditorSettings& GetInstance();

	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_editor_settings";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return Localization::GetString("interface_main_menu_item_editor_settings");
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	WindowEditorSettings() = default;
	~WindowEditorSettings() = default;
	WindowEditorSettings(const WindowEditorSettings&) = delete;
	WindowEditorSettings& operator=(const WindowEditorSettings&) = delete;
public:
	//variables
	bool showWindow = false;

private:
	ImVec2 defSize{ 400.0f, 400.0f };
};