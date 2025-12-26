#pragma once

#include "Light.h"
#include "DirectionalLightParams.h"

#include "interfaces/IEntity/ILight/IDirectionalLight/IDirectionalLight.h"

#include "glad_wrapper.h"

using namespace adr;

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class DirectionalLight : public virtual Light, public virtual IDirectionalLight
{
public:
	ENTITYMANAGER_API virtual ~DirectionalLight() = default;

	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<DirectionalLight>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

	//Getters
	ENTITYMANAGER_API glm::mat4 GetLightSpaceMatrix() override;
public:
	//light matrix
	glm::mat4 lightSpaceMatrix;
private:
	//properties
	std::shared_ptr<IDirectionalLightParams> params;
};