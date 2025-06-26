#pragma once

#include "Entity.h"
#include "CameraParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Camera : public Entity
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
		return std::make_shared<Camera>(*this);
	}

	//properties
	ENTITYMANAGER_API EntityParams* GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

	//Camera specs
	ENTITYMANAGER_API void AddPosition(glm::vec3 posAdd, bool isPlaying);
	ENTITYMANAGER_API void AddRotation(float _yaw, float _pitch, bool isPlaying);

	ENTITYMANAGER_API void SetPosition(glm::vec3 newPos, bool isPlaying);
	ENTITYMANAGER_API void SetRotation(float _yaw, float _pitch, bool isPlaying);

	//first -> yaw, second -> pitch
	ENTITYMANAGER_API std::pair<float, float> GetYawPitch();
	ENTITYMANAGER_API CameraProjection        GetProjectionType() { return params->projectionType; };
	ENTITYMANAGER_API float                   GetFOV() { return params->fov; };
private:
	//properties
	std::shared_ptr<CameraParams> params;

	float yaw = -90.0f;
	float pitch = 0.0f;
};