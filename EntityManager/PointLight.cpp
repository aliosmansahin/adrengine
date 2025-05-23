#include "pch.h"
#include "PointLight.h"

/*
PURPOSE: Initializes the entity
*/
bool PointLight::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to PointLightParams to use its properties
	auto casted = std::dynamic_pointer_cast<PointLightParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<PointLightParams*>(params)");
		return false;
	}
	this->params = casted;

	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void PointLight::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
void PointLight::Update()
{
	/*
		Here we are calculating the real position, rotation and scale from properties.
		We will use these vectors to draw it in the space.
		If the entity has a parent, add the paramters to real* which is currently from parent's real*.
		Otherwise real* will be properties directly
	*/
	if (params.get()) {
		if (params->parent.get()) {
			realPos += glm::vec3(params->x, params->y, params->z);
			realRot += glm::vec3(params->rx, params->ry, params->rz);
			realSca *= glm::vec3(params->sx, params->sy, params->sz);
		}
		else {
			realPos = glm::vec3(params->x, params->y, params->z);
			realRot = glm::vec3(params->rx, params->ry, params->rz);
			realSca = glm::vec3(params->sx, params->sy, params->sz);
		}
	}
}

/*
PURPOSE: Draws the entity from the position of the camera.
	This entity draws nothing
*/
void PointLight::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* PointLight::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json PointLight::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;
}
