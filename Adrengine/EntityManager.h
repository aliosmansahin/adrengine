#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "Sprite2D.h"
#include "Logger.h"

class EntityManager
{
private:
	//singleton
	EntityManager() = default;
	~EntityManager() = default;
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
public:
	//getter for the instance
	static EntityManager& GetInstance();
public:
	//main funcs
	bool InitEntityManager();
	void DrawEntities();
	void UpdateEntities();
	void ReleaseEntityManager();

	//management for an entity
	std::string CreateEntity();
	bool RemoveEntity(std::string which);

	//status
	size_t GetEntityCount() { return entities.size(); }
private:
	std::unordered_map<std::string, std::unique_ptr<Entity>> entities;
};