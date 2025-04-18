#pragma once

#include "Entity.h"

struct Sprite2DParams : public EntityParams {
	float x = 0.0f, y = 0.0f;
	float sx = 1.0f, sy = 1.0f;
	float rx = 0.0f, ry = 0.0f;
	unsigned int texture;
	float r, b, g, a;
};

class Sprite2D : public Entity
{
public:
	bool CreateEntity(EntityParams* entityParams) override;
	void DeleteEntity() override;
	void Update() override;
	void Draw() override;
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	Sprite2DParams* params;
};

