#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Entity.h"
#include "Sprite2D.h"
#include "WindowScene.h"

class EntityManager
{
public:
	//main funcs
	bool InitEntityManager();
	void DrawEntities();
	void UpdateEntities();
	void ReleaseEntityManager();

	//management for an entity
	std::string CreateEntity(std::string type, Entity* parent = nullptr, bool newEntity = true);
	bool RemoveEntity(std::string which);

	//status
	size_t GetEntityCount() { return entities.size(); }
	std::map<std::string, std::shared_ptr<Entity>>& GetEntities() { return entities; }
private:
	std::map<std::string, std::shared_ptr<Entity>> entities;
};