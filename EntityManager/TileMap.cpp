#include "pch.h"
#include "TileMap.h"

/*
PURPOSE: Initializes the entity
*/
bool TileMap::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to Sprite2DParams to use its properties
	auto casted = std::dynamic_pointer_cast<TileMapParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<TileMapParams*>(params)");
		return false;
	}
	this->params = casted;

	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void TileMap::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
void TileMap::Update()
{
}

/*
PURPOSE: Draws the entity.
	This entity draws a texture
*/
void TileMap::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* TileMap::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json TileMap::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;

}
