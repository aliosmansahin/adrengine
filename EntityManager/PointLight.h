#pragma once

#include "Light.h"
#include "PointLightParams.h"
#include "ShaderManager.h"

#include "interfaces/IEntity/ILight/IPointLight/IPointLight.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class PointLight : public virtual Light, public virtual IPointLight
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
		return std::make_shared<PointLight>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

	ENTITYMANAGER_API float GetFarPlane() override;

private:
	//point light parameters and matrices
	float far_plane = 1000.0f;
	std::vector<glm::mat4> shadowTransforms;

private:
	//properties
	std::shared_ptr<IPointLightParams> params;
};