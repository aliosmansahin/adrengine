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

#ifdef _ADRPHYSICS // Ignored to use Bullet Physics for now
class Object; // Forward declaration

struct CollisionContact
{
    glm::vec3 contactPoint;
    glm::vec3 normal; // from A to B
    float penetrationDepth;
};

struct CollisionManifold
{
    std::vector<CollisionContact> contacts;
};

struct Face {
    glm::vec3 normal;
    std::vector<glm::vec3> verts;
};

struct CollisionData {
    CollisionManifold manifold;
    Object* objA;
    Object* objB;
    glm::vec3 posA;
    glm::vec3 posB;
    OBB obbA;
    OBB obbB;
};


class Collision
{
public:
	//Collision detection and response
    PHYSICS_API static void ResolveCollisionImpulse(
        Physical* A, Physical* B,
        CollisionManifold& data,
        glm::vec3& posA, glm::vec3& posB,
        OBB& obbA, OBB& obbB,
        double restitution,
        double frictionCoeff
    );
    PHYSICS_API static bool TestOBBOBB(const OBB& obbA, const OBB& obbB, CollisionManifold& outData);

    PHYSICS_API static void PositionalCorrection(Physical* A, Physical* B, glm::vec3& posA, glm::vec3& posB, const CollisionManifold& data, float percent = 0.2f, float slop = 0.01f);
private:
	static int GetReferenceFaceIndex(const OBB& obb, const glm::vec3& normal, bool& outFlip);
    static Face GetFace(const OBB& obb, int index, bool flipNormal = false);
	static bool ClipPolygonAgainstPlane(std::vector<glm::vec3>& inputVerts, const glm::vec3& planeNormal, float planeDist, std::vector<glm::vec3>& outVerts);
    static void AddEdgeFaceContacts(const glm::vec3& edgeStart, const glm::vec3& edgeEnd,
        const Face& referenceFace,
        const glm::vec3& collisionNormal,
		CollisionManifold& manifold);
    static std::vector<glm::vec3> ClipIncidentFaceToReferenceFace(const Face& incidentFace, const Face& referenceFace);
};

#endif