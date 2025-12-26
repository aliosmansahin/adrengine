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
#include "interfaces/IEntity/ISprite2D/IFlipBook/IFlipBook.h"

#include "AssetDatabase.h"

class WindowFlipBookEdit
{
public:
	//drawer
	INTERFACEMANAGER_API void DrawWindow();

	//getter for the intance
	INTERFACEMANAGER_API static WindowFlipBookEdit& GetInstance();

	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_flipbook_edit";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Edit Flip Book";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	WindowFlipBookEdit() = default;
	~WindowFlipBookEdit() = default;
	WindowFlipBookEdit(const WindowFlipBookEdit&) = delete;
	WindowFlipBookEdit& operator=(const WindowFlipBookEdit&) = delete;
public:
	bool showWindow = false;
	std::shared_ptr<IFlipBook> edittingFlipBook = nullptr;

private:
	ImVec2 defSize{ 600.0f, 600.0f };
};