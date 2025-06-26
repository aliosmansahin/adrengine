#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

class Physical
{
public:
	PHYSICS_API void Update(float deltaTime);
	PHYSICS_API void ApplyForce(glm::vec3 force);
	PHYSICS_API void ApplyImpulse(glm::vec3 force);
	PHYSICS_API bool IsPhysical();
	PHYSICS_API void SetPhysical(bool physical);
	PHYSICS_API glm::vec3 GetVelocity();
	PHYSICS_API void SetVelocity(glm::vec3 velo);
	PHYSICS_API void Reset();
	PHYSICS_API void ApplyForceAtPoint(const glm::vec3& appliedForce, const glm::vec3& point, const glm::vec3& centerOfMass);
public:
	float mass = 2.0f;
	float inverseMass = 1.0f;
	float friction = 0.3f;
	float bounciness = 0.2f;
private:
	float frictionCoefficient = 0.2f;
	float linearDamping = 0.05f;
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 force = glm::vec3(0.0f);
	glm::vec3 impulse = glm::vec3(0.0f);
	bool physicsEffects = true;
};

