#include "pch.h"
#include "Camera.h"

/*
PURPOSE: Initializes the entity
*/
ENTITYMANAGER_API bool Camera::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to CameraParams to use its properties
	auto casted = std::dynamic_pointer_cast<CameraParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<CameraParams*>(params)");
		return false;
	}
	this->params = casted;

	return true;
}

/*
PURPOSE: Unitializes the entity
*/
ENTITYMANAGER_API void Camera::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
ENTITYMANAGER_API void Camera::Update()
{
}

/*
PURPOSE: Draws the entity.
	This entity draws a texture
*/
ENTITYMANAGER_API void Camera::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
ENTITYMANAGER_API EntityParams* Camera::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
ENTITYMANAGER_API nlohmann::json Camera::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;
}
