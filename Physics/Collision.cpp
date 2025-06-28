#include "pch.h"
#include "Collision.h"

/*
Returns axes of obb
*/
PHYSICS_API std::array<glm::vec3, 3> Collision::GetAxes(const OBB& obb)
{
    return {
        obb.orientation[0],
        obb.orientation[1],
        obb.orientation[2],
    };
}

/*
PURPOSE: Projects obb onto given axis
*/
void Collision::ProjectOBB(const OBB& obb, const glm::vec3& axis, float& minOut, float& maxOut)
{
    float centerProj = glm::dot(obb.center, axis);
    // Projection of halfExtents (abs)
    float r = 0.0f;
    for (int i = 0; i < 3; i++) {
        r += obb.halfExtents[i] * fabs(glm::dot(obb.orientation[i], axis));
    }
    minOut = centerProj - r;
    maxOut = centerProj + r;
}

/*
PURPOSE: Runs a collision test between two OBB shape
*/
bool Collision::TestOBBvsOBB(const OBB& a, const OBB& b, CollisionManifold& outManifold)
{
    const auto axesA = GetAxes(a); // Local axes of a
    const auto axesB = GetAxes(b); // Local axes of b

    std::vector<glm::vec3> testAxes;

    // 3 A axes + 3 B axes
    testAxes.insert(testAxes.end(), axesA.begin(), axesA.end());
    testAxes.insert(testAxes.end(), axesB.begin(), axesB.end());

    // 9 cross axis
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            glm::vec3 axis = glm::cross(axesA[i], axesB[j]);
            float lenSq = glm::pow(glm::length(axis), 2.0f);
            if (lenSq > 1e-6f) {
                testAxes.push_back(glm::normalize(axis));
            }
        }
    }

    float minOverlap = std::numeric_limits<float>::max();
    glm::vec3 smallestAxis(0.0f);

    for (const glm::vec3& axisRaw : testAxes) {
        // To be sure normalization
        glm::vec3 axis = glm::normalize(axisRaw);

        float minA, maxA;
        float minB, maxB;

        ProjectOBB(a, axis, minA, maxA);
        ProjectOBB(b, axis, minB, maxB);

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < 0.0f) {
            // There is a space between two shapes when they projected onto this axis
            outManifold.isColliding = false;
            return false;
        }

        if (overlap < minOverlap) {
            // Export results
            minOverlap = overlap;
            glm::vec3 axisFixed = axis;
            glm::vec3 dirAB = b.center - a.center;
            
            if (glm::dot(dirAB, axisFixed) > 0.0f) {
                axisFixed = -axisFixed;
            }
            smallestAxis = axisFixed;
        }
    }

    //Fill CollisionManifold
    outManifold.normal = smallestAxis;
    outManifold.penetration = minOverlap;
    
    glm::vec3 pointOnA = a.center + outManifold.normal * (a.halfExtents - outManifold.penetration * 0.5f);
    glm::vec3 pointOnB = b.center + outManifold.normal * (b.halfExtents - outManifold.penetration * 0.5f);
    outManifold.contactPoint = 0.5f * (pointOnA + pointOnB);
    outManifold.isColliding = true;

    return true;
}
