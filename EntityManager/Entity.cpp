#include "pch.h"
#include "Entity.h"

/*
PURPOSE: Initializes the entity
*/
bool Entity::CreateEntity(std::shared_ptr<IEntityParams> params)
{
    this->params = std::dynamic_pointer_cast<EntityParams>(params);
    return true;
}

/*
PURPOSE: Unitializes the entity
*/
void Entity::DeleteEntity()
{
    if(params->GetVisualScript())
        params->GetVisualScript()->ReleaseScript();
}

/*
PURPOSE: Updates the entity
*/
void Entity::Update()
{
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
std::shared_ptr<IEntityParams> Entity::GetEntityParams()
{
    return params;
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

/*
PURPOSE: Sets transform params of this entity
*/
ENTITYMANAGER_API void Entity::SetTransform(glm::vec3 newRealPos, glm::vec3 newRealRot, glm::vec3 newRealSca)
{
    realPos = newRealPos;
    realRot = newRealRot;
    realSca = newRealSca;
}
