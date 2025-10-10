#pragma once

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "RigidBody.h"

#include "interfaces/IPhysics/IPhysics.h"

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

class Physics : public IPhysics
{
public:
	//General
	virtual ~Physics() = default;
	PHYSICS_API virtual void Init() = 0;
	PHYSICS_API virtual void Update(float dt) = 0;
	PHYSICS_API virtual void Shutdown() = 0;

	//Rigid Body
	PHYSICS_API virtual void AddRigidBody(std::shared_ptr<IRigidBody>& rigidBody) = 0;
	PHYSICS_API virtual void RemoveRigidBody(std::shared_ptr<IRigidBody>& rigidBody) = 0;
	PHYSICS_API virtual void UpdateEntityTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot) = 0;
	PHYSICS_API virtual void UpdateRigidbodyTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot) = 0;

	//Scene
	PHYSICS_API virtual void StartEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;
	PHYSICS_API virtual void EndEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;

	//Apply props
	PHYSICS_API virtual void ApplyPropsForRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;
private:
};