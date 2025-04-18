#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"
#include "ShaderManager.h"

struct EntityParams {
	virtual ~EntityParams() {}
};

class Entity
{
public:
	//main funcs
	virtual bool CreateEntity(EntityParams* params) = 0;
	virtual void DeleteEntity() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

