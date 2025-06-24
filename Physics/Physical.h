#pragma once

#include <glm/glm.hpp>
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
private:
	float mass = 1.0f;
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 force = glm::vec3(0.0f);
	glm::vec3 impulse = glm::vec3(0.0f);
	bool physicsEffects = true;
};

