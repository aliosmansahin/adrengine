#pragma once

#include <glm/glm.hpp>

#include "../IRigidBody/IRigidBody.h"

class IPhysics
{
public:
	virtual ~IPhysics() = default;
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Shutdown() = 0;

	//Rigid Body
	virtual void AddRigidBody(std::shared_ptr<IRigidBody>& rigidBody) = 0;
	virtual void RemoveRigidBody(std::shared_ptr<IRigidBody>& rigidBody) = 0;
	virtual void UpdateEntityTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot) = 0;
	virtual void UpdateRigidbodyTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot) = 0;

	//Scene
	virtual void StartEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;
	virtual void EndEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;

	//Apply props
	virtual void ApplyPropsForRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;
};

