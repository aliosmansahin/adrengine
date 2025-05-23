#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"

#include "EntityParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Entity
{
public:
	//main funcs
	ENTITYMANAGER_API virtual bool CreateEntity(std::shared_ptr<EntityParams> params);
	ENTITYMANAGER_API virtual void DeleteEntity();
	ENTITYMANAGER_API virtual void Update();
	ENTITYMANAGER_API virtual void Draw(glm::vec3 currentSceneCameraPos);

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API virtual std::shared_ptr<Entity> clone() const {
		return std::make_shared<Entity>(*this);
	}
	
	//properties
	ENTITYMANAGER_API virtual EntityParams*  GetEntityParams();

	//json
	ENTITYMANAGER_API virtual nlohmann::json ToJson();
private:
	//properties
	std::shared_ptr<EntityParams> params;
public:
	//Space vectors
	glm::vec3 realPos;
	glm::vec3 realRot;
	glm::vec3 realSca;
};
