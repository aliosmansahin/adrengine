#include "pch.h"
#include "TileMapParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void TileMapParams::FromJson(const nlohmann::json& j, std::string& projectDir, IScene* scene)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j, projectDir, scene);

	//Set some properties
	textureId = j.value("texture-id", "");
	
	//Set the texture of the sprite from the asset database
	auto assetTexture = AssetDatabase::GetInstance().GetTexture(textureId).get();
	if (assetTexture) {
		texture = assetTexture->texture;
	}
}