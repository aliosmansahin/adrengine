#include "pch.h"
#include "Entity.h"

/*
PURPOSE: Initializes the entity
*/
bool Entity::CreateEntity(std::shared_ptr<EntityParams> params)
{
    this->params = params;
    return true;
}

/*
PURPOSE: Unitializes the entity
*/
void Entity::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
void Entity::Update()
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
PURPOSE: Draws the entity.
	This entity doesn't draw anything
*/
void Entity::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* Entity::GetEntityParams()
{
    return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json Entity::ToJson()
{
    nlohmann::json j;

    if (params) {
        j = params->ToJson();
    }
    return j;
}
