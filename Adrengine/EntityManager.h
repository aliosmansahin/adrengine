#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "Entity.h"
#include "Logger.h"

class EntityManager
{
public:
	//main funcs
	bool InitEntityManager();
	void DrawEntities();
	void UpdateEntities();

	//management for an entity
	std::string CreateEntity();
	bool RemoveEntity(std::string which);

	//status
	size_t GetEntityCount() { return entities.size(); }
private:
	std::unordered_map<std::string, std::unique_ptr<Entity>> entities;
};