#pragma once

#include "Entity.h"
#include "ObjectParams.h"
#include "ShaderManager.h"

#include "Timer.h"

#include "interfaces/IEntity/IObject/IObject.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Object : public virtual Entity, public virtual IObject
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	ENTITYMANAGER_API void AddImpulse(glm::vec3 impulse) override;
	ENTITYMANAGER_API void ResetPhysics() override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<Object>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	ENTITYMANAGER_API glm::mat4 GetModelMatrix() override {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, realPos);
		model = glm::rotate(model, glm::radians(realRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(realRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(realRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, realSca);
		return model;
	}

	//Rigidody
	ENTITYMANAGER_API std::shared_ptr<IRigidBody>& GetRigidBody() override;
	ENTITYMANAGER_API void SetRigidBody(std::shared_ptr<IRigidBody> rigidBody) override;
	
	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

private:
	glm::vec3 lastPos = glm::vec3(0.0f);

	//physics
	std::shared_ptr<IRigidBody> rigidBody = nullptr;

private:
	//properties
	std::shared_ptr<IObjectParams> params;
};

