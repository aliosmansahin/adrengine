#include "Sprite2DParams.h"

void Sprite2DParams::FromJson(const nlohmann::json& j) {
	EntityParams::FromJson(j);
	r = j["r"];
	g = j["g"];
	b = j["b"];
	a = j["a"];
	std::string tmpTextureId = j.value("texture-id", "");
	textureId = tmpTextureId;
	if (AssetDatabase::GetInstance().GetTexture(textureId).get()) {
		texture = AssetDatabase::GetInstance().GetTexture(textureId)->texture;
	}
}