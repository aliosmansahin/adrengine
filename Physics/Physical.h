#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

class Physical
{
public:
	PHYSICS_API void Update(float deltaTime, glm::vec3& pos, glm::vec3& rot);
	PHYSICS_API void ApplyForce(glm::vec3 force);
	PHYSICS_API bool IsPhysical();
	PHYSICS_API void SetPhysical(bool physical);
	PHYSICS_API glm::vec3 GetVelocity();
	PHYSICS_API void SetVelocity(glm::vec3 velo);
	PHYSICS_API void Reset();
public:
	float mass = 1.0f;
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 angularVelocity = glm::vec3(0.0f);
	float linearDamping = 0.4f;
	float angularDamping = 0.4f;
	float friction = 0.3f;
	float restitution = 1.0f;// 0 stick, 1 jump
	bool physicsEffects = true;
private:
	glm::vec3 force = glm::vec3(0.0f);
};

