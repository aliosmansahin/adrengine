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

class Physics {
public:
	//General
	virtual ~Physics() = default;
	PHYSICS_API virtual void Init() = 0;
	PHYSICS_API virtual void Update(float dt) = 0;
	PHYSICS_API virtual void Shutdown() = 0;

	//Rigid Body
	PHYSICS_API virtual void AddRigidBody(std::string id) = 0;
	PHYSICS_API virtual void RemoveRigidBody(std::string id) = 0;
	PHYSICS_API virtual void UpdateEntityTransforms(std::string id, glm::vec3& pos, glm::vec3& rot) = 0;
	PHYSICS_API virtual void UpdateRigidbodyTransforms(std::string id, glm::vec3& pos, glm::vec3& rot) = 0;

	//Scene
	PHYSICS_API virtual void StartEmulation() = 0;
	PHYSICS_API virtual void EndEmulation() = 0;

	//Kinematic
	PHYSICS_API virtual bool GetIsKinematic(std::string id) = 0;
	PHYSICS_API virtual void SetIsKinematic(std::string id, bool isKinematic) = 0;
private:
};