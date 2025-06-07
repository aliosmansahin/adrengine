#include "pch.h"
#include "FlipBook.h"

/*
PURPOSE: Initializes the entity
*/
ENTITYMANAGER_API bool FlipBook::CreateEntity(std::shared_ptr<EntityParams> params)
{
    //Cast EntityParams to FlipBookParams to use its properties
    auto casted = std::dynamic_pointer_cast<FlipBookParams>(params);
    if (!casted) {
        Logger::Log("E", "Casting failed at dynamic_cast<FlipBookParams*>(params)");
        return false;
    }
    this->params = casted;

    return true;
}

/*
PURPOSE: Unitializes the entity
*/
ENTITYMANAGER_API void FlipBook::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
ENTITYMANAGER_API void FlipBook::Update()
{
    
}

/*
PURPOSE: Draws the entity.
    This entity draws a sprite from a sprite array which is being displayed
*/
ENTITYMANAGER_API void FlipBook::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
ENTITYMANAGER_API EntityParams* FlipBook::GetEntityParams()
{
    return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
    "Entity" creates entity json, "Object" creates object json.
*/
ENTITYMANAGER_API nlohmann::json FlipBook::ToJson()
{
	nlohmann::json j; //For the entity

	if (params) {
		j = params->ToJson();
	}

	return j;
}
