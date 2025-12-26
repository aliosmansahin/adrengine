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

#include <functional>

enum class ModalType {
	Question,
	Alert,
	Input
};

class WindowModalDialog
{
public:
	//main
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API void ShowModalQuestion(std::string title, std::string caption, std::function<void()> yesCallback);
	INTERFACEMANAGER_API void ShowModalAlert(std::string title, std::string caption);
	INTERFACEMANAGER_API void ShowModalInput(std::string title, std::string caption, std::function<void(std::string)> okCallback);
	INTERFACEMANAGER_API void OpenModalIfNeeded();
	INTERFACEMANAGER_API void CloseModalWindow();

private:
	void SetupModal(std::string title, std::string caption);

public:
	//Singleton
	INTERFACEMANAGER_API static WindowModalDialog& GetInstance();

private:
	WindowModalDialog() = default;
	~WindowModalDialog() = default;
	WindowModalDialog(const WindowModalDialog&) = delete;
	WindowModalDialog& operator=(const WindowModalDialog&) = delete;

private:
	bool willShowModal = false;

	std::function<void()> yesCallback;
	std::function<void(std::string)> okCallback;

	ModalType type;
	std::string title;
	std::string caption;
};

