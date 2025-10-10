#pragma once

#include <btBulletDynamicsCommon.h>

#include <unordered_map>
#include <iostream>

#include "Physics.h"
#include "RigidBody.h"

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

class BulletPhysics : public Physics
{
public:
	//General
	PHYSICS_API void Init() override;
	PHYSICS_API void Update(float dt) override;
	PHYSICS_API void Shutdown() override;

	//Rigid body
	PHYSICS_API void AddRigidBody(std::shared_ptr<IRigidBody>& rigidBody) override;
	PHYSICS_API void RemoveRigidBody(std::shared_ptr<IRigidBody>& rigidBody) override;
	PHYSICS_API void UpdateEntityTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot) override;
	PHYSICS_API void UpdateRigidbodyTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot) override;

	//Scene
	PHYSICS_API void StartEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody) override;
	PHYSICS_API void EndEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody) override;

	//Apply props
	PHYSICS_API void ApplyPropsForRigidBody(std::shared_ptr<IRigidBody> rigidBody) override;
private:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* broadphase;
	btSequentialImpulseConstraintSolver* solver;
};

