#pragma once

#include "Entity.h"
#include "Sprite2DParams.h"

class Sprite2DParams;

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

