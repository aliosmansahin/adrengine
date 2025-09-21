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
	PHYSICS_API void AddRigidBody(std::string id) override;
	PHYSICS_API void RemoveRigidBody(std::string id) override;
	PHYSICS_API void UpdateEntityTransforms(std::string id, glm::vec3& pos, glm::vec3& rot) override;
	PHYSICS_API void UpdateRigidbodyTransforms(std::string id, glm::vec3& pos, glm::vec3& rot) override;

	//Scene
	PHYSICS_API void StartEmulation() override;
	PHYSICS_API void EndEmulation() override;

	//Kinematic
	PHYSICS_API bool GetIsKinematic(std::string id) override;
	PHYSICS_API void SetIsKinematic(std::string id, bool isKinematic) override;
private:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* broadphase;
	btSequentialImpulseConstraintSolver* solver;

private:
	std::unordered_map<std::string, std::unique_ptr<RigidBody>> rigidbodies;
};

