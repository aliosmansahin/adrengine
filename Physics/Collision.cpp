#include "pch.h"
#include "Collision.h"

/*
PURPOSE: Returns the peak point that views a direction of an obb
*/
PHYSICS_API float Collision::GetSupportDepth(const OBB& obb, const glm::vec3& dir)
{
    float depth = 0.0f;
    for (int i = 0; i < 3; ++i) {
        depth += obb.halfExtents[i] * std::abs(glm::dot(obb.orientation[i], dir));
    }
    return depth;
}

/*
PURPOSE: Returns a face that views to dir of an obb
*/
PHYSICS_API Face Collision::GetFaceInDirection(const OBB& obb, const glm::vec3& dir)
{
    float maxDot = -1.0f;
    int bestAxis = -1;
    float bestSign = 1.0f;

    for (int i = 0; i < 3; ++i) {
        float d = glm::dot(obb.orientation[i], dir);
        float absD = fabs(d);
        if (absD > maxDot) {
            maxDot = absD;
            bestAxis = i;
            bestSign = (d >= 0.0f) ? 1.0f : -1.0f;
        }
    }

    glm::vec3 normal = bestSign * obb.orientation[bestAxis];

    glm::vec3 faceCenter = obb.center + normal * obb.halfExtents[bestAxis];

    int u = (bestAxis + 1) % 3;
    int v = (bestAxis + 2) % 3;

    glm::vec3 right = obb.orientation[u] * obb.halfExtents[u];
    glm::vec3 up = obb.orientation[v] * obb.halfExtents[v];

    Face face;
    face.normal = normal;

    face.vertices[0] = faceCenter + right + up;
    face.vertices[1] = faceCenter - right + up;
    face.vertices[2] = faceCenter - right - up;
    face.vertices[3] = faceCenter + right - up;

    return face;
}

/*
PURPOSE: Returns the point of the average of colliding quat
*/
PHYSICS_API glm::vec3 Collision::ComputeQuatCentroid(const glm::vec3 v[4])
{
    return (v[0] + v[1] + v[2] + v[3]) * 0.25f;
}

PHYSICS_API std::vector<glm::vec3> Collision::ClipPolygonAgainsPlane(const std::vector<glm::vec3>& vertices, const glm::vec3& planeNormal, const glm::vec3& planePoint)
{
    std::vector<glm::vec3> result;
    const float epsilon = 1e-5f;

    for (size_t i = 0; i < vertices.size(); ++i) {
        const glm::vec3& curr = vertices[i];
        const glm::vec3& prev = vertices[(i + vertices.size() - 1) % vertices.size()];

        float distCurr = glm::dot(planeNormal, curr - planePoint);
        float distPrev = glm::dot(planeNormal, prev - planePoint);

        bool currInside = distCurr >= -epsilon;
        bool prevInside = distPrev >= -epsilon;

        if (currInside) {
            if (!prevInside) {
                float t = distPrev / (distPrev - distCurr);
                glm::vec3 intersection = prev + t * (curr - prev);

                result.push_back(intersection);
            }
            result.push_back(curr);
        }
        else if (prevInside) {
            float t = distPrev / (distPrev - distCurr);
            glm::vec3 intersection = prev + t * (curr - prev);
            result.push_back(intersection);
        }
    }

    return result;
}

PHYSICS_API std::vector<glm::vec3> Collision::ComputeContactPolygon(const Face& faceA, const Face& faceB)
{
    float areaA = ComputeFaceArea(faceA);
    float areaB = ComputeFaceArea(faceB);

    const Face* bigFace = &faceA;
    const Face* smallFace = &faceB;

    if (areaB > areaA) {
        bigFace = &faceB;
        smallFace = &faceA;
    }

    std::vector<glm::vec3> polygon(bigFace->vertices, bigFace->vertices + 4);

    // Küçük yüzeyin her kenarýna göre clip et
    for (int i = 0; i < 4; ++i) {
        glm::vec3 p0 = smallFace->vertices[i];
        glm::vec3 p1 = smallFace->vertices[(i + 1) % 4];
        glm::vec3 edgeDir = glm::normalize(p1 - p0);

        //Normal yönü düzeltilmiþ
        glm::vec3 edgeNormal = glm::normalize(glm::cross(smallFace->normal, edgeDir));

        // Yüzeye dýþarý bakan düzlem yönünü korumak için kontrol
        if (glm::dot(edgeNormal, bigFace->normal) < 0.0f)
            edgeNormal = -edgeNormal;

        polygon = ClipPolygonAgainsPlane(polygon, edgeNormal, p0);
        if (polygon.empty())
            return {};
    }

    // Büyük yüzeyin kenarlarýna göre de clip et
    for (int i = 0; i < 4; ++i) {
        glm::vec3 p0 = bigFace->vertices[i];
        glm::vec3 p1 = bigFace->vertices[(i + 1) % 4];
        glm::vec3 edgeDir = glm::normalize(p1 - p0);

        //Normal yönü düzeltilmiþ
        glm::vec3 edgeNormal = glm::normalize(glm::cross(bigFace->normal, edgeDir));

        // Dýþa bakacak þekilde kontrol
        if (glm::dot(edgeNormal, smallFace->normal) < 0.0f)
            edgeNormal = -edgeNormal;

        polygon = ClipPolygonAgainsPlane(polygon, edgeNormal, p0);
        if (polygon.empty())
            return {};
    }

    return polygon;
}

PHYSICS_API glm::vec3 Collision::ComputePolygonCentroid(const std::vector<glm::vec3>& poly)
{
    glm::vec3 sum(0.0f);
    for (const auto& p : poly) sum += p;
    return sum / static_cast<float>(poly.size());
}

PHYSICS_API float Collision::ComputePolygonArea(const std::vector<glm::vec3>& poly)
{
    glm::vec3 normal = glm::normalize(glm::cross(poly[1] - poly[0], poly[2] - poly[0]));
    float area = 0.0f;

    for (size_t i = 1; i + 1 < poly.size(); ++i) {
        glm::vec3 edge1 = poly[i] - poly[0];
        glm::vec3 edge2 = poly[i + 1] - poly[0];

        glm::vec3 cross = glm::cross(edge1, edge2);
        area += glm::dot(cross, normal) * 0.5f;
    }

    return fabs(area);
}

PHYSICS_API float Collision::ComputeFaceArea(const Face& face)
{
    const glm::vec3& v0 = face.vertices[0];
    const glm::vec3& v1 = face.vertices[1];
    const glm::vec3& v2 = face.vertices[2];
    const glm::vec3& v3 = face.vertices[3];

    float area1 = glm::length(glm::cross(v1 - v0, v2 - v0)) * 0.5f;
    float area2 = glm::length(glm::cross(v2 - v0, v3 - v0)) * 0.5f;

    return area1 + area2;
}

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
    float r = GetSupportDepth(obb, axis);
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
    
    Face faceA = GetFaceInDirection(a, outManifold.normal);
    Face faceB = GetFaceInDirection(b, -outManifold.normal);

    auto contactPolygon = ComputeContactPolygon(faceA, faceB);
    for (int i = 0; i < contactPolygon.size(); ++i) {
        std::cout << "contactPolygon[" << i << "] = " << glm::to_string(contactPolygon[i]) << std::endl;
    }
    if (!contactPolygon.empty()) {
        glm::vec3 centroid = ComputePolygonCentroid(contactPolygon);

        if (fabs(ComputePolygonArea(contactPolygon) - ComputeFaceArea(faceA)) < 0.01f) {
            outManifold.contactPointA = ComputeQuatCentroid(faceA.vertices);
        }
        else {
            glm::vec3 planeANormal = faceA.normal;
            glm::vec3 planeAPoint = faceA.vertices[0];

            float dA = glm::dot(planeANormal, centroid - planeAPoint);
            outManifold.contactPointA = centroid - dA * planeANormal;
        }
        std::cout << ComputePolygonArea(contactPolygon) << " " << ComputeFaceArea(faceA) << " " << ComputeFaceArea(faceB) << std::endl;
        if (fabs(ComputePolygonArea(contactPolygon) - ComputeFaceArea(faceB)) < 0.01f) {
            outManifold.contactPointB = ComputeQuatCentroid(faceB.vertices);
        }
        else {
            glm::vec3 planeBNormal = faceB.normal;
            glm::vec3 planeBPoint = faceB.vertices[0];

            float dB = glm::dot(planeBNormal, centroid - planeBPoint);
            outManifold.contactPointB = centroid - dB * planeBNormal;
        }
    }

    outManifold.isColliding = true;

    return true;
}
