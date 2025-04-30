#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"
#include "EntityManager.h"
#include "WindowEntityProperties.h"
#include "WindowAddEntity.h"

class WindowScene
{
public:
	void DrawWindow();
	void DrawEntity(Entity& entity, int depth = 0);
	void CheckForChildren(bool& toChildren, Entity* entity, std::string droppedId);
	static WindowScene& GetInstance();
private:
	WindowScene() = default;
	~WindowScene() = default;
	WindowScene(const WindowScene&) = delete;
	WindowScene& operator=(const WindowScene&) = delete;
private:
	std::vector<std::function<void()>> deferredEntityChanges;
public:
	bool showWindow = true;
	bool focused = false;
	bool deletePressed = false;
	bool pendingDelete = false;
	std::string selectedId = "";
	Entity* addParent = nullptr;
};