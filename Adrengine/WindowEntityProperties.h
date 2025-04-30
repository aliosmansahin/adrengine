#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "EntityManager.h"

class WindowEntityProperties
{
public:
	void DrawWindow();
	void SelectEntity(Entity* entity);
	static WindowEntityProperties& GetInstance();
private:
	WindowEntityProperties() = default;
	~WindowEntityProperties() = default;
	WindowEntityProperties(const WindowEntityProperties&) = delete;
	WindowEntityProperties& operator=(const WindowEntityProperties&) = delete;
public:
	bool showWindow = true;
	Entity* currentEntity;
};