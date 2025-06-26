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
    //glm::vec3 axes[15];
    //int axisCount = 0;

    //// 3 lokal eksen A
    //for (int i = 0; i < 3; i++)
    //    axes[axisCount++] = a.orientation[i];

    //// 3 lokal eksen B
    //for (int i = 0; i < 3; i++)
    //    axes[axisCount++] = b.orientation[i];

    //// 9 çapraz çarpým (A[i] x B[j])
    //for (int i = 0; i < 3; i++) {
    //    for (int j = 0; j < 3; j++) {
    //        glm::vec3 axis = glm::cross(a.orientation[i], b.orientation[j]);
    //        if (glm::length(axis) > 1e-6f)
    //            axes[axisCount++] = glm::normalize(axis);
    //    }
    //}

    //float minOverlap = std::numeric_limits<float>::max();
    //glm::vec3 smallestAxis;

    //for (int i = 0; i < axisCount; i++) {
    //    glm::vec3 axis = glm::normalize(axes[i]);
    //    float minA, maxA, minB, maxB;
    //    ProjectOBB(a, axes[i], minA, maxA);
    //    ProjectOBB(b, axes[i], minB, maxB);

    //    if ((maxA < minB || maxB < minA)) {
    //        float overlapDepth = std::min(maxA, maxB) - std::max(minA, minB);
    //        if (overlapDepth < minOverlap) {
    //            minOverlap = overlapDepth;
    //            smallestAxis = axis;
    //        }
    //    }
    //    else { return false; }
    //}
    //
    //// MTV hesapla
    //// MTV yönü, A'dan B'ye doðru olacak þekilde ayarlanmalý
    //glm::vec3 d = b.center - a.center;
    //if (glm::dot(d, smallestAxis) < 0)
    //    smallestAxis = -smallestAxis;

    //outManifold.normal = smallestAxis;
    //outManifold.penetration = minOverlap;

    //return true; // Çarpýþma var













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
    glm::vec3 d = b.center - a.center;

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
        }
    }

    // Sonuçlarý dýþa aktar
    glm::vec3 dirAB = b.center - a.center;
    if (glm::length(dirAB) < 1e-6f) {
        // Eðer merkezler çok yakýnsa, smallestAxis'ý olduðu gibi býrak veya alternatif bir eksen kullan
    }
    else if (glm::dot(smallestAxis, dirAB) < 0.0f) {
        smallestAxis = -smallestAxis;
    }

    outManifold.normal = glm::normalize(smallestAxis);
    outManifold.penetration = minOverlap;
    outManifold.isColliding = true;

    std::cout << "b - a: " << glm::to_string(dirAB) << std::endl;
    std::cout << "normal: " << glm::to_string(outManifold.normal) << std::endl;
    std::cout << "dot: " << glm::dot(dirAB, outManifold.normal) << std::endl;

    return true;
}
