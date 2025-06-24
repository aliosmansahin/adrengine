#pragma once

#include "CollisionShape.h"

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

class Collision
{
public:
	PHYSICS_API bool CheckCollision(const glm::vec3 posA, const CollisionShape& colA, const glm::vec3& posB, const CollisionShape& colB);
private:
	PHYSICS_API bool AABBvsAABB(const glm::vec3 posA, const CollisionShape& colA, const glm::vec3& posB, const CollisionShape& colB);
};