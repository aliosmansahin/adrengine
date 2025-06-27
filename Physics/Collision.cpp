#include "pch.h"
#include "Collision.h"

PHYSICS_API std::array<glm::vec3, 3> Collision::GetAxes(const OBB& obb)
{
    return {
        obb.orientation[0],
        obb.orientation[1],
        obb.orientation[2],
    };
}

void Collision::ProjectOBB(const OBB& obb, const glm::vec3& axis, float& minOut, float& maxOut)
{
    float centerProj = glm::dot(obb.center, axis);
    // Yarý boyutlarýn projeksiyonu (mutlak deðer)
    float r = 0.0f;
    for (int i = 0; i < 3; i++) {
        r += obb.halfExtents[i] * fabs(glm::dot(obb.orientation[i], axis));
    }
    minOut = centerProj - r;
    maxOut = centerProj + r;
}

bool Collision::TestOBBvsOBB(const OBB& a, const OBB& b, CollisionManifold& outManifold)
{
    const auto axesA = GetAxes(a); // A'nýn local eksenleri
    const auto axesB = GetAxes(b); // B'nin local eksenleri

    std::vector<glm::vec3> testAxes;

    // 3 A ekseni + 3 B ekseni
    testAxes.insert(testAxes.end(), axesA.begin(), axesA.end());
    testAxes.insert(testAxes.end(), axesB.begin(), axesB.end());

    // 9 çapraz eksen
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            glm::vec3 axis = glm::cross(axesA[i], axesB[j]);
            float lenSq = glm::pow(glm::length(axis), 2);
            if (lenSq > 1e-6f) {
                testAxes.push_back(glm::normalize(axis));
            }
        }
    }

    float minOverlap = std::numeric_limits<float>::max();
    glm::vec3 smallestAxis(0.0f);

    for (const glm::vec3& axisRaw : testAxes) {
        // Eksen zaten normalize edildi mi emin ol
        glm::vec3 axis = glm::normalize(axisRaw);

        float minA, maxA;
        float minB, maxB;

        ProjectOBB(a, axis, minA, maxA);
        ProjectOBB(b, axis, minB, maxB);

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap <= 0.0f) {
            // Ayrýþan eksen çarpýþma yok
            outManifold.isColliding = false;
            return false;
        }

        if (overlap < minOverlap) {
            minOverlap = overlap;
            smallestAxis = axis;
            // Sonuçlarý dýþa aktar
            glm::vec3 dirAB = b.center - a.center;
            if (glm::dot(dirAB, axis) > 0.0f) {
                smallestAxis = -axis;
            }
        }
    }

    outManifold.normal = smallestAxis;
    outManifold.penetration = minOverlap;
    outManifold.isColliding = true;

    return true;
}
