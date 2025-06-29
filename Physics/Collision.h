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
	glm::vec3 contactPointA = glm::vec3(0.0f);
	glm::vec3 contactPointB = glm::vec3(0.0f);
	glm::vec3 contactPoint = glm::vec3(0.0f);
	float penetration = 0.0f;
};

struct Face {
	glm::vec3 normal;
	glm::vec3 vertices[4];
};

class Collision
{
private:
	PHYSICS_API static float GetSupportDepth(const OBB& obb, const glm::vec3& dir);
	PHYSICS_API static Face GetFaceInDirection(const OBB& obb, const glm::vec3& dir);
	PHYSICS_API static glm::vec3 ComputeQuatCentroid(const glm::vec3 v[4]);
	PHYSICS_API static std::vector<glm::vec3> ClipPolygonAgainsPlane(const std::vector<glm::vec3>& vertices, const glm::vec3& planeNormal, const glm::vec3& planePoint);
	PHYSICS_API static std::vector<glm::vec3> ComputeContactPolygon(const Face& faceA, const Face& faceB);
	PHYSICS_API static glm::vec3 ComputePolygonCentroid(const std::vector<glm::vec3>& poly);
	PHYSICS_API static float ComputePolygonArea(const std::vector<glm::vec3>& poly);
	PHYSICS_API static float ComputeFaceArea(const Face& face);
public:
	PHYSICS_API static std::array<glm::vec3, 3> GetAxes(const OBB& obb);
	PHYSICS_API static void ProjectOBB(const OBB& obb, const glm::vec3& axis, float& minOut, float& maxOut);
	PHYSICS_API static bool TestOBBvsOBB(const OBB& a, const OBB& b, CollisionManifold& outManifold);
};