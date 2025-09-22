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

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

//Forward declarations
struct RigidBodyProperties;
class RigidBody;

class Physics {
public:
	//General
	virtual ~Physics() = default;
	PHYSICS_API virtual void Init() = 0;
	PHYSICS_API virtual void Update(float dt) = 0;
	PHYSICS_API virtual void Shutdown() = 0;

	//Rigid Body
	PHYSICS_API virtual void AddRigidBody(RigidBody*& rigidBody) = 0;
	PHYSICS_API virtual void RemoveRigidBody(RigidBody*& rigidBody) = 0;
	PHYSICS_API virtual void UpdateEntityTransforms(RigidBody* rigidBody, glm::vec3& pos, glm::vec3& rot) = 0;
	PHYSICS_API virtual void UpdateRigidbodyTransforms(RigidBody* rigidBody, glm::vec3& pos, glm::vec3& rot) = 0;

	//Scene
	PHYSICS_API virtual void StartEmulationForRigidBody(RigidBody* rigidBody) = 0;
	PHYSICS_API virtual void EndEmulationForRigidBody(RigidBody* rigidBody) = 0;

	//Apply props
	PHYSICS_API virtual void ApplyPropsForRigidBody(RigidBody* rigidBody) = 0;
private:
};