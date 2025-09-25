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
	PHYSICS_API void AddRigidBody(RigidBody*& rigidBody) override;
	PHYSICS_API void RemoveRigidBody(RigidBody*& rigidBody) override;
	PHYSICS_API void UpdateEntityTransforms(RigidBody* rigidBody, glm::vec3& pos, glm::vec3& rot) override;
	PHYSICS_API void UpdateRigidbodyTransforms(RigidBody* rigidBody, glm::vec3& pos, glm::vec3& rot) override;

	//Scene
	PHYSICS_API void StartEmulationForRigidBody(RigidBody* rigidBody) override;
	PHYSICS_API void EndEmulationForRigidBody(RigidBody* rigidBody) override;

	//Apply props
	PHYSICS_API void ApplyPropsForRigidBody(RigidBody* rigidBody) override;
private:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* broadphase;
	btSequentialImpulseConstraintSolver* solver;
};

