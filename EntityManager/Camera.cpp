#include "pch.h"
#include "Camera.h"

/*
PURPOSE: Initializes the entity
*/
ENTITYMANAGER_API bool Camera::CreateEntity(std::shared_ptr<IEntityParams> params)
{
	//Cast EntityParams to CameraParams to use its properties
	auto casted = std::dynamic_pointer_cast<ICameraParams>(params);
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
ENTITYMANAGER_API std::shared_ptr<IEntityParams> Camera::GetEntityParams()
{
	return params;
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

/*
PURPOSE: Adds position vector to camera's position
*/
ENTITYMANAGER_API void Camera::AddPosition(glm::vec3 posAdd, bool isPlaying)
{
	glm::vec3 newPosition = params->GetPosition() + posAdd;
	if (isPlaying) {
		params->SetRuntimePosition(newPosition);
	}
	else {
		params->SetEditorPosition(newPosition);
	}
}

/*
PURPOSE: Adds rotation vector to camera's rotation, and calculates rotation vector
*/
ENTITYMANAGER_API void Camera::AddRotation(float _yaw, float _pitch, bool isPlaying)
{
	yaw += _yaw;
	pitch += _pitch;

	//Limit the pitch
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//Convert yaw and pitch to a vector
	glm::vec3 direction{};
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::vec3 eye = glm::normalize(direction);

	if (isPlaying) {
		params->SetRuntimeRotation(eye);
	}
	else {
		params->SetRuntimeRotation(eye);
	}
}

/*
PURPOSE: Sets camera's position
*/
ENTITYMANAGER_API void Camera::SetPosition(glm::vec3 newPos, bool isPlaying)
{
	if (isPlaying)
		params->SetRuntimePosition(newPos);
	else
		params->SetEditorPosition(newPos);
}

/*
PURPOSE: Sets camera's rotation and calculates rotation vector
*/
ENTITYMANAGER_API void Camera::SetRotation(float _yaw, float _pitch, bool isPlaying)
{
	yaw = _yaw;
	pitch = _pitch;

	//Limit the pitch
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//Convert yaw and pitch to a vector
	glm::vec3 direction{};
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::vec3 eye = glm::normalize(direction);

	if (isPlaying) {
		params->SetRuntimeRotation(eye);
	}
	else {
		params->SetRuntimeRotation(eye);
	}
}

/*
PURPOSE: Returns yaw and pitch in a pair
	first -> yaw
	second -> pitch
*/
ENTITYMANAGER_API std::pair<float, float> Camera::GetYawPitch()
{
	return { yaw, pitch };
}
