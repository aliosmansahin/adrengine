#pragma once

#include "Entity.h"
#include "ObjectParams.h"
#include "ShaderManager.h"

#include "Physical.h"
#include "Collision.h"
#include "Timer.h"

#include "interfaces/IEntity/IObject/IObject.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Object : public Entity, public IObject
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<EntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	ENTITYMANAGER_API void AddImpulse(glm::vec3 impulse) override;
	ENTITYMANAGER_API void ResetPhysics();

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<Entity> clone() const override {
		return std::make_shared<Object>(*this);
	}

	//properties
	ENTITYMANAGER_API EntityParams* GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;
	std::shared_ptr<CollisionShape> collisionShape; //TODO: Add entity properties to control it, serialization
	glm::vec3 lastPos = glm::vec3(0.0f);

	//physics
	std::shared_ptr<Physical> physical;
private:

	//properties
	std::shared_ptr<ObjectParams> params;

};

