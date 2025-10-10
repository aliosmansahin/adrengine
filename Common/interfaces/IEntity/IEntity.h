#pragma once

#include "IEntityParams.h"

class IEntity
{
public:
	//main funcs
	virtual bool CreateEntity(std::shared_ptr<IEntityParams> params) = 0;
	virtual void DeleteEntity() = 0;
	virtual void Update() = 0;
	virtual void Draw(glm::vec3 currentSceneCameraPos) = 0;

	/*
	PURPOSE: Clones the entity and return it
	*/
	virtual std::shared_ptr<IEntity> clone() const = 0;

	//properties
	virtual std::shared_ptr<IEntityParams> GetEntityParams() = 0;

	//json
	virtual nlohmann::json ToJson() = 0;

	/*
	PURPOSE: Sets realPos, realRot, realSca params
	*/
	virtual void SetTransform(glm::vec3 newRealPos, glm::vec3 newRealRot, glm::vec3 newRealSca) = 0;

	virtual ~IEntity() = default;
};