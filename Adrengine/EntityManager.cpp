#include "EntityManager.h"

bool EntityManager::InitEntityManager()
{
	Logger::Log("P", "Initalizing entity manager");
	return true;
}

void EntityManager::DrawEntities()
{
	for (auto& entity : entities) {
		entity.second->Draw();
	}
}

void EntityManager::UpdateEntities()
{
	for (auto& entity : entities) {
		entity.second->Update();
	}
}

std::string EntityManager::CreateEntity()
{
	Entity* entity = new Entity();
	if (!entity->CreateEntity())
		return "";

	std::string entityId = std::to_string(entities.size());
	entities.insert(std::pair<std::string, std::unique_ptr<Entity>>(entityId, std::unique_ptr<Entity>(entity)));

	std::string str = "Created new entity \"";
	str += entityId;
	str += "\"";

	Logger::Log("P", str.c_str());
	return entityId;
}

bool EntityManager::RemoveEntity(std::string which)
{
	auto entity = entities.find(which);
	if (entity == entities.end()) {
		std::string str = "There is not any entity which has given id \"";
		str += which;
		str += "\"";
		Logger::Log("E", str.c_str());
		return false;
	}

	entity->second->DeleteEntity();

	entities.erase(entity);

	std::string str = "Removed entity \"";
	str += which;
	str += "\"";
	Logger::Log("P", str.c_str());
	return true;
}
