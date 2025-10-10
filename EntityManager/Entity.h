#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"

#include "EntityParams.h"

#include "interfaces/IEntity/IEntity.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Entity : public virtual IEntity
{
public:
	//main funcs
	ENTITYMANAGER_API virtual bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API virtual void DeleteEntity() override;
	ENTITYMANAGER_API virtual void Update() override;
	ENTITYMANAGER_API virtual void Draw(glm::vec3 currentSceneCameraPos) override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API virtual std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<Entity>(*this);
	}
	
	//properties
	ENTITYMANAGER_API virtual std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API virtual nlohmann::json ToJson() override;

	/*
	PURPOSE: Sets realPos, realRot, realSca params
	*/
	ENTITYMANAGER_API void SetTransform(glm::vec3 newRealPos, glm::vec3 newRealRot, glm::vec3 newRealSca) override;

private:
	//properties
	std::shared_ptr<IEntityParams> params;

protected:
	//Space vectors
	glm::vec3 realPos;
	glm::vec3 realRot;
	glm::vec3 realSca;
};
