#pragma once

#include <string>
#include <vector>

#include "nlohmann_json/json.hpp"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Entity;
class VisualScript;

class EntityParams {
public:
	//Destructor
	ENTITYMANAGER_API virtual ~EntityParams() = default;

	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API virtual std::string GetType() {
		return "Entity";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API virtual std::shared_ptr<EntityParams> clone() const {
		return std::make_shared<EntityParams>(*this);
	}

	//Some properties for the entity
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float sx = 1.0f, sy = 1.0f, sz = 1.0f;
	float rx = 0.0f, ry = 0.0f, rz = 0.0f;
	std::string id = "";
	std::string name = "";

	//Stores the parent entity
	std::shared_ptr<Entity> parent;
	std::string parentId = "";

	//Stores children
	std::vector<std::shared_ptr<Entity>> children;

	//Stores the script of the entity
	std::string scriptId = "";
	VisualScript* script = nullptr;

	//json
	ENTITYMANAGER_API virtual nlohmann::json ToJson();
	ENTITYMANAGER_API virtual void			 FromJson(const nlohmann::json& j);
};