#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "nlohmann_json/json.hpp"

#include "interfaces/IEntity/IEntityParams.h"
#include "interfaces/IScene/IScene.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

/*
	We don't have to have implementation of the Entity and VisualScript class
	Because we will use them as a pointer, also we won't use functions of them
*/
class Entity;
class VisualScript;

class EntityParams : public IEntityParams {
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

	//runtime
	ENTITYMANAGER_API void ResetRuntimeValues();

	/*
	PURPOSE: Returns the position vector of the entity
	*/
	ENTITYMANAGER_API glm::vec3 GetPosition() const override {
		return runtimePosition;
	}

	/*
	PURPOSE: Sets the position vector of the entity
	*/
	ENTITYMANAGER_API void SetRuntimePosition(glm::vec3 vector) override {
		runtimePosition = vector;
	}
	ENTITYMANAGER_API void SetEditorPosition(glm::vec3 vector) {
		editorPosition = vector;
		runtimePosition = vector;
	}

	//Some properties for the entity
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
	std::shared_ptr<VisualScript> script;

	//json
	ENTITYMANAGER_API virtual nlohmann::json ToJson();
	ENTITYMANAGER_API virtual void			 FromJson(const nlohmann::json& j, std::string& projectDir, IScene* scene);

private:
	glm::vec3 runtimePosition = glm::vec3(0.0f);
	glm::vec3 editorPosition = glm::vec3(0.0f);
};