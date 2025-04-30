#pragma once

#include "Entity.h"
#include "ShaderManager.h"
#include "Graphics.h"

struct Sprite2DParams : public EntityParams {
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
		j["g"] = r;
		j["b"] = r;
		j["a"] = r;
		if(!textureId.empty())
			j["texture-id"] = textureId;
		j["type"] = GetType();

		return j;
	};

	void FromJson(const nlohmann::json& j) override {
		EntityParams::FromJson(j);
		r = j["r"];
		g = j["g"];
		b = j["b"];
		a = j["a"];
		textureId = j.value("texture-id", "");
		texture = Graphics::GetInstance().GetTexture(textureId);
	}
	std::string textureId = "";
	unsigned int texture = 0;
	float r = 0, b = 0, g = 0, a = 0;
};

class Sprite2D : public Entity
{
public:
	bool CreateEntity(std::shared_ptr<EntityParams> params) override;
	void DeleteEntity() override;
	void Update() override;
	void Draw() override;
	std::shared_ptr<Entity> clone() const override {
		return std::make_shared<Sprite2D>(*this);
	}
	EntityParams* GetEntityParams() override;
	nlohmann::json ToJson() override;
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	std::shared_ptr<Sprite2DParams> params;
};

