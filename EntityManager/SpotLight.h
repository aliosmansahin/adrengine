#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Entity.h"
#include "SpotLightParams.h"
#include "ShaderManager.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class SpotLight : public Entity
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<EntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<Entity> clone() const override {
		return std::make_shared<SpotLight>(*this);
	}

	//properties
	ENTITYMANAGER_API EntityParams* GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;
public:
	unsigned int depthMapFBO;
	unsigned int depthMap;
	glm::mat4 lightSpaceMatrix;
	int index = 0;
private:
	//properties
	std::shared_ptr<SpotLightParams> params;
};