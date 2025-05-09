#pragma once

#include "EntityParams.h"
#include "AssetDatabase.h"

class Sprite2DParams : public EntityParams {
public:
	std::string GetType() override {
		return "Sprite2D";
	}
	std::shared_ptr<EntityParams> clone() const override {
		return std::make_shared<Sprite2DParams>(*this);
	}

	nlohmann::json ToJson() override {
		auto j = EntityParams::ToJson();
		//texture path
		j["r"] = r;
		j["g"] = g;
		j["b"] = b;
		j["a"] = a;
		if (!textureId.empty())
			j["texture-id"] = textureId;
		j["type"] = GetType();

		return j;
	};

	void FromJson(const nlohmann::json& j) override;

	std::string textureId = "";
	unsigned int texture = 0;
	float r = 0, b = 0, g = 0, a = 0;
};