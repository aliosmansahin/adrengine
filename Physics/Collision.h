#pragma once

#include "CollisionShape.h"
#include <array>
#include <iostream>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

struct CollisionManifold {
	bool isColliding = false;
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec3 contactPoint = glm::vec3(0.0f);
	float penetration = 0.0f;
};

class Collision
{
public:
	PHYSICS_API static std::array<glm::vec3, 3> GetAxes(const OBB& obb);
	PHYSICS_API static void ProjectOBB(const OBB& obb, const glm::vec3& axis, float& minOut, float& maxOut);
	PHYSICS_API static bool TestOBBvsOBB(const OBB& a, const OBB& b, CollisionManifold& outManifold);
};