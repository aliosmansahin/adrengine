#include "pch.h"
#include "Collision.h"

/*
PURPOSE: Calls collision check function depends on collision types
*/
PHYSICS_API bool Collision::CheckCollision(const glm::vec3 posA, const CollisionShape& colA, const glm::vec3& posB, const CollisionShape& colB)
{
    glm::vec3 minA = posA - colA.aabb.halfExtents;
    glm::vec3 maxA = posA + colA.aabb.halfExtents;

    glm::vec3 minB = posB - colB.aabb.halfExtents;
    glm::vec3 maxB = posB + colB.aabb.halfExtents;

    return (maxA.x > minB.x && minA.x < maxB.x) &&
        (maxA.y > minB.y && minA.y < maxB.y) &&
        (maxA.z > minB.z && minA.z < maxB.z);
}

/*
PURPOSE: Checks collision AABB and AABB
*/
PHYSICS_API bool Collision::AABBvsAABB(const glm::vec3 posA, const CollisionShape& colA, const glm::vec3& posB, const CollisionShape& colB)
{
	if (colA.type == CollisionType::AABB && colB.type == CollisionType::AABB) {
		return AABBvsAABB(posA, colA, posB, colB);
	}

	return false;
}
