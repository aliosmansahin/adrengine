#include "pch.h"
#include "Sprite2DParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void Sprite2DParams::FromJson(const nlohmann::json& j)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j);

	//Set some properties
	r = j["r"];
	g = j["g"];
	b = j["b"];
	a = j["a"];
	textureId = j.value("texture-id", "");

	//Set the texture of the sprite from the asset database
	auto assetTexture = AssetDatabase::GetInstance().GetTexture(textureId).get();
	if (assetTexture) {
		texture = assetTexture->texture;
	}
}