#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"

#include "EntityParams.h"

class Entity
{
public:
	//main funcs
	virtual bool CreateEntity(std::shared_ptr<EntityParams> params);
	virtual void DeleteEntity();
	virtual void Update();
	virtual void Draw();
	virtual std::shared_ptr<Entity> clone() const {
		return std::make_shared<Entity>(*this);
	}
	virtual EntityParams* GetEntityParams();
	virtual nlohmann::json ToJson();
private:
	std::shared_ptr<EntityParams> params;
};
