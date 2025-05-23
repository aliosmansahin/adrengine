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
#include "SceneManager.h"
#include "EntityManager.h"

class WindowAddEntity
{
public:
	//main funcions
	INTERFACEMANAGER_API void DrawWindow(
		std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
		std::string& projectDir,
		std::shared_ptr<Entity>& windowSceneAddParent
	);
	INTERFACEMANAGER_API static WindowAddEntity& GetInstance();
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
};