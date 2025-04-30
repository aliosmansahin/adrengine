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
	if (WindowScene::GetInstance().focused && WindowScene::GetInstance().deletePressed) {
		RemoveEntity(WindowScene::GetInstance().selectedId);
		WindowEntityProperties::GetInstance().SelectEntity(nullptr);
	}
	if (WindowScene::GetInstance().pendingDelete) {
		if (!RemoveEntity(WindowScene::GetInstance().selectedId))
			std::cout << "error" << std::endl;
		WindowEntityProperties::GetInstance().SelectEntity(nullptr);
		WindowScene::GetInstance().pendingDelete = false;
	}

	for (auto& entity : entities) {
		entity.second->Update();
	}
}

void EntityManager::ReleaseEntityManager()
{
	entities.clear();
	Logger::Log("P", "Released entity manager");
}

std::string EntityManager::CreateEntity(std::string type, Entity* parent, bool newEntity)
{
	int index = 0;
	std::string entityId = "";
	while (true) {
		entityId = type + std::to_string(index);
		auto entity = entities.find(entityId);
		if (entity == entities.end())
			break;
		++index;
	}

	auto typeIter = Engine::GetInstance().entityTypes.find(type);
	if (typeIter == Engine::GetInstance().entityTypes.end()) {
		std::string str = "Could not find entity type \"";
		str += entityId;
		str += "\"";

		Logger::Log("P", str.c_str());
		return "";
	}

	auto entity = typeIter->second.first->clone();
	auto params = typeIter->second.second->clone();

	params->parent = std::shared_ptr<Entity>(parent);
	params->id = entityId;
	params->name = entityId;

	if (!entity->CreateEntity(params))
		return "";
	
	if (parent) {
		parent->GetEntityParams()->children.push_back(entity);
		WindowScene::GetInstance().addParent = nullptr;
	}
	
	entities.insert(std::pair<std::string, std::shared_ptr<Entity>>(entityId, entity));


	std::string str = "Created new entity \"";
	str += entityId;
	str += "\"";

	Logger::Log("P", str.c_str());
	return entityId;
}

bool EntityManager::RemoveEntity(std::string which)
{
	auto entityIter = entities.find(which);

	if (entityIter == entities.end()) {
		std::string str = "There is not any entity which has given id \"";
		str += which;
		str += "\"";
		Logger::Log("E", str.c_str());
		return false;
	}

	auto entity = entityIter->second.get();

	auto& parent = entity->GetEntityParams()->parent;
	if (parent) {
		auto& children = parent->GetEntityParams()->children;
		children.erase(std::remove_if(children.begin(), children.end(), [&](std::shared_ptr<Entity>& child) {
			return child->GetEntityParams()->id == entity->GetEntityParams()->id;
			}), children.end());
	}

	auto& children = entity->GetEntityParams()->children;
	for (auto& child : children) {
		RemoveEntity(child->GetEntityParams()->id);
	}

	entity->DeleteEntity();

	entities.erase(entityIter);

	std::string str = "Removed entity \"";
	str += which;
	str += "\"";
	Logger::Log("P", str.c_str());
	return true;
}
