#include "pch.h"
#include "FlipBookParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
ENTITYMANAGER_API void FlipBookParams::FromJson(const nlohmann::json& j, std::string& projectDir)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j, projectDir);

	//Set some properties
	textureId = j.value("texture-id", "");

	//Set the texture of the sprite from the asset database
	auto assetTexture = AssetDatabase::GetInstance().GetTexture(textureId).get();
	if (assetTexture) {
		texture = assetTexture->texture;
	}
	
	//Showing frames
	frameWait = j.value("frame-wait", 0.1f);
	loop = j.value("loop", false);
}
