#pragma once

#include <glm/glm.hpp>

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
	PHYSICS_API bool IsPhysical();
	PHYSICS_API void SetPhysical(bool physical);
private:
	float mass = 1.0f;
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 force = glm::vec3(0.0f);
	bool physicsEffects = true;
};

