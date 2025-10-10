#pragma once

#include "Light.h"
#include "SpotLightParams.h"
#include "ShaderManager.h"

#include "interfaces/IEntity/ILight/ISpotLight/ISpotLight.h"


#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class SpotLight : public virtual Light, public virtual ISpotLight
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<SpotLight>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

	//Getters
	ENTITYMANAGER_API glm::mat4 GetLightSpaceMatrix() override;
private:
	//light matrix
	glm::mat4 lightSpaceMatrix;
private:
	//properties
	std::shared_ptr<ISpotLightParams> params;
};