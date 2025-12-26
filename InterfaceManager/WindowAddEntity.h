#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "interfaces/IEngine/IEngine.h"
#include "interfaces/ISceneManager/ISceneManager.h"

#include "ServiceLocator.h"

class WindowAddEntity
{
public:
	//main funcions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API static WindowAddEntity& GetInstance();
	INTERFACEMANAGER_API std::string GetWindowId() {
		return "window_add_entity";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return "Add Entity";
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	WindowAddEntity() = default;
	~WindowAddEntity() = default;
	WindowAddEntity(const WindowAddEntity&) = delete;
	WindowAddEntity& operator=(const WindowAddEntity&) = delete;
public:
	//variables
	bool showWindow = false;
	bool shouldFocus = true;
	std::vector<std::string> types;
	char typeBuf[32] = {};

private:
	ImVec2 defSize{ 400.0f, 400.0f };
};