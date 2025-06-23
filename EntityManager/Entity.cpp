#include "pch.h"
#include "Entity.h"

#include "VisualScript.h"

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
    if(params->script)
        params->script->ReleaseScript();
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
