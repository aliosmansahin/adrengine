#pragma once

#include "CollisionShape.h"
#include "Physical.h"
#include <array>
#include <iostream>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

struct CollisionData
{
    glm::vec3 contactPoint;
    glm::vec3 normal; // from A to B
    float penetrationDepth;
};

class Collision
{
public:
	//Collision detection and response
    PHYSICS_API static void ResolveCollisionImpulse(
        Physical* A, Physical* B,
        CollisionData& data,
        OBB& obbA, OBB& obbB,
        double restitution,
        double frictionCoeff
    );
    PHYSICS_API static bool TestOBBOBB(const OBB& obbA, const OBB& obbB, CollisionData& outData);
private:
	static void ProjectOntoAxis(const OBB& box, const glm::vec3& axis, float& outMin, float& outMax);
	static float OBBProjectedRadius(const OBB& box, const glm::vec3& axis);
    static void PositionalCorrection(Physical* A, Physical* B, const CollisionData& data, float percent = 0.2f, float slop = 0.01f);
};