#include "pch.h"
#include "Collision.h"
#include <algorithm>

#ifdef _ADRPHYSICS // Ignored to use Bullet Physics for now
// Collision impulse resolution
void Collision::ResolveCollisionImpulse(
    Physical* A, Physical* B,
    CollisionManifold& data,
    glm::vec3& posA, glm::vec3& posB,
    OBB& obbA, OBB& obbB,
    double restitution,
    double frictionCoeff
) {
    if (!A->IsPhysical() && !B->IsPhysical()) return;

    float e = (float)restitution;

	float sf = frictionCoeff; // TODO: static friction
	float df = frictionCoeff; // TODO: dynamic friction

	int contactCount = static_cast<int>(data.contacts.size());
	//std::cout << "Contact count: " << contactCount << std::endl;

	// Inertia tensor in world space for A
	glm::vec3 fullExtentsA = obbA.halfExtents * 2.0f;
    glm::vec3 inertiaA = (A->GetMass() / 12.0f) * glm::vec3(
        fullExtentsA.y * fullExtentsA.y + fullExtentsA.z * fullExtentsA.z,
        fullExtentsA.x * fullExtentsA.x + fullExtentsA.z * fullExtentsA.z,
        fullExtentsA.x * fullExtentsA.x + fullExtentsA.y * fullExtentsA.y
	);

    glm::vec3 invInertiaVecA = glm::vec3(
        inertiaA.x > 0.0f ? 1.0f / inertiaA.x : 0.0f,
        inertiaA.y > 0.0f ? 1.0f / inertiaA.y : 0.0f,
        inertiaA.z > 0.0f ? 1.0f / inertiaA.z : 0.0f
	);

    glm::mat3 invInertiaA = obbA.orientation * glm::mat3(
        glm::vec3(invInertiaVecA.x, 0.0f, 0.0f),
        glm::vec3(0.0f, invInertiaVecA.y, 0.0f),
        glm::vec3(0.0f, 0.0f, invInertiaVecA.z)
    ) * glm::transpose(obbA.orientation);
    
	// Inertia tensor in world space for B
    glm::vec3 fullExtentsB = obbB.halfExtents * 2.0f;
    glm::vec3 inertiaB = (B->GetMass() / 12.0f) * glm::vec3(
        fullExtentsB.y * fullExtentsB.y + fullExtentsB.z * fullExtentsB.z,
        fullExtentsB.x * fullExtentsB.x + fullExtentsB.z * fullExtentsB.z,
        fullExtentsB.x * fullExtentsB.x + fullExtentsB.y * fullExtentsB.y
	);
    glm::vec3 invInertiaVecB = glm::vec3(
        inertiaB.x > 0.0f ? 1.0f / inertiaB.x : 0.0f,
        inertiaB.y > 0.0f ? 1.0f / inertiaB.y : 0.0f,
		inertiaB.z > 0.0f ? 1.0f / inertiaB.z : 0.0f
	);
    glm::mat3 invInertiaB = obbB.orientation * glm::mat3(
        glm::vec3(invInertiaVecB.x, 0.0f, 0.0f),
        glm::vec3(0.0f, invInertiaVecB.y, 0.0f),
        glm::vec3(0.0f, 0.0f, invInertiaVecB.z)
    ) * glm::transpose(obbB.orientation);


	//std::cout << "orientation B: " << glm::to_string(obbB.orientation) << std::endl;

	std::vector <glm::vec3> impulseList(contactCount, glm::vec3(0.0f));
	std::vector <glm::vec3> raList(contactCount, glm::vec3(0.0f));
	std::vector <glm::vec3> rbList(contactCount, glm::vec3(0.0f));
	std::vector <float> jList(contactCount, 0.0f);
	std::vector <glm::vec3> frictionImpulseList(contactCount, glm::vec3(0.0f));

    /* LINEAR AND ANGULAR VELOCITY */
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec3 normal = data.contacts[i].normal;

		auto& contact = data.contacts[i];

        glm::vec3 rA = contact.contactPoint - obbA.center;
        glm::vec3 rB = contact.contactPoint - obbB.center;

        glm::vec3 angularLinearVelocityA = glm::cross(A->angularVelocity, rA);
        glm::vec3 angularLinearVelocityB = glm::cross(B->angularVelocity, rB);

        glm::vec3 relativeVelocity =
            (B->velocity + angularLinearVelocityB) -
            (A->velocity + angularLinearVelocityA);

        float contactVelocityMag = glm::dot(relativeVelocity, normal);

        if (contactVelocityMag > 0.0f)
        {
            continue;
        }

        glm::vec3 raCrossN = glm::cross(rA, normal);
        glm::vec3 rbCrossN = glm::cross(rB, normal);

        float denom = A->GetInvMass() + B->GetInvMass()
            //;
            + glm::dot(normal, glm::cross(invInertiaA * raCrossN, rA) + glm::cross(invInertiaB * rbCrossN, rB));
            /*glm::dot(invInertiaA * raCrossN, normal) +
            glm::dot(invInertiaB * rbCrossN, normal);*/

        float j = -(1.0f + e) * contactVelocityMag;
        j /= denom;

        if (fabs(j) < 1e-16f) continue;

		glm::vec3 impulse = j * normal;

		impulseList[i] = impulse;
		raList[i] = rA;
		rbList[i] = rB;
		jList[i] = j;
    }
    

    for (int i = 0; i < contactCount; i++)
    {
        glm::vec3 impulse = impulseList[i];
        glm::vec3 ra = raList[i];
        glm::vec3 rb = rbList[i];

		//std::cout << "Impulse: " << glm::to_string(impulse) << std::endl;

        if (A->IsPhysical()) {
            A->velocity += -impulse * A->GetInvMass();
            A->angularVelocity += invInertiaA * -glm::cross(ra, impulse);
        }
        if (B->IsPhysical()) {
            B->velocity += impulse * B->GetInvMass();
            B->angularVelocity += invInertiaB * glm::cross(rb, impulse);
			std::cout << "torqueB: " << glm::to_string(glm::cross(rb, impulse)) << std::endl;
        }

        //std::cout << glm::to_string(ra) << " <<< " << glm::to_string(B->velocity) << std::endl;
    }

    // impulse sonrasý
    //std::cout << "B velocity after impulse: " << glm::to_string(B->velocity) << std::endl;

    /* FRICTION */
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec3 normal = data.contacts[i].normal;

		auto& contact = data.contacts[i];

        glm::vec3 rA = contact.contactPoint - posA;
        glm::vec3 rB = contact.contactPoint - posB;

        glm::vec3 angularLinearVelocityA = glm::cross(A->angularVelocity, rA);
        glm::vec3 angularLinearVelocityB = glm::cross(B->angularVelocity, rB);

        glm::vec3 relativeVelocity =
            (B->velocity + angularLinearVelocityB) -
            (A->velocity + angularLinearVelocityA);

        glm::vec3 tangent = relativeVelocity - glm::dot(relativeVelocity, normal) * normal;

        if (glm::length2(tangent) < 1e-36f)
        {
            continue;
        }
        else
        {
            tangent = glm::normalize(tangent);
        }

        glm::vec3 raCrossT = glm::cross(rA, tangent);
        glm::vec3 rbCrossT = glm::cross(rB, tangent);

        float denom = A->GetInvMass() + B->GetInvMass() +
		    glm::dot(tangent, glm::cross(invInertiaA * raCrossT, rA) + glm::cross(invInertiaB * rbCrossT, rB));
           /* glm::dot(invInertiaA * raCrossT, tangent) +
            glm::dot(invInertiaB * rbCrossT, tangent);*/


        float jt = -glm::dot(relativeVelocity, tangent);
        jt /= denom;

        if (fabs(jt) < 1e-16f) continue;

        glm::vec3 frictionImpulse;

        float j = jList[i];

        if (glm::abs(jt) <= j * sf)
        {
            frictionImpulse = jt * tangent;
			std::cout << "Static friction applied: " << glm::to_string(frictionImpulse) << std::endl;
        }
        else
        {
            frictionImpulse = -j * tangent * df;
			std::cout << "Dynamic friction applied: " << glm::to_string(frictionImpulse) << std::endl;
        }

        frictionImpulseList[i] = frictionImpulse;
        raList[i] = rA;
        rbList[i] = rB;
    }

    for (int i = 0; i < contactCount; i++)
    {
        glm::vec3 frictionImpulse = frictionImpulseList[i];
        glm::vec3 ra = raList[i];
        glm::vec3 rb = rbList[i];

        if (A->IsPhysical()) {
            A->velocity += -frictionImpulse * A->GetInvMass();
            A->angularVelocity += invInertiaA * -glm::cross(ra, frictionImpulse);
        }
        if (B->IsPhysical()) {
            B->velocity += frictionImpulse * B->GetInvMass();
            B->angularVelocity += invInertiaB * glm::cross(rb, frictionImpulse);
        }
    }
}


void Collision::PositionalCorrection(Physical* A, Physical* B, glm::vec3& posA, glm::vec3& posB, const CollisionManifold& data, float percent, float slop)
{
	if (data.contacts.empty()) return;

    float penetration = glm::max(data.contacts[0].penetrationDepth - slop, 0.0f);
    if (penetration <= 0.0f) return;

    glm::vec3 normal = data.contacts[0].normal;

    if (!A->IsPhysical())
        posB += normal * penetration * percent;
    else if (!B->IsPhysical())
        posA -= normal * penetration * percent;
    else {
        posA -= normal * penetration * 0.5f * percent;
        posB += normal * penetration * 0.5f * percent;
    }
}
int Collision::GetReferenceFaceIndex(const OBB& obb, const glm::vec3& normal, bool& outFlip)
{
    float maxDot = -FLT_MAX;
    int faceIndex = -1;

    for (int i = 0; i < 3; ++i) {
        float dot = glm::dot(glm::normalize(obb.orientation[i]), normal);
        if (fabs(dot) > maxDot) {
            maxDot = fabs(dot);
            faceIndex = i;
            outFlip = (dot < 0.0f);
        }
    }
    return faceIndex;
}

Face Collision::GetFace(const OBB& obb, int index, bool flipNormal)
{
	glm::vec3 normal = obb.orientation[index];
	if (flipNormal) normal = -normal;

	glm::vec3 up = obb.orientation[(index + 1) % 3];
	glm::vec3 right = obb.orientation[(index + 2) % 3];

	glm::vec3 center = obb.center + normal * obb.halfExtents[index];

	glm::vec3 halfUp = up * obb.halfExtents[(index + 1) % 3];
	glm::vec3 halfRight = right * obb.halfExtents[(index + 2) % 3];

	Face face;
	face.normal = normal;

	face.verts.push_back(center + halfUp + halfRight);
	face.verts.push_back(center + halfUp - halfRight);
	face.verts.push_back(center - halfUp - halfRight);
	face.verts.push_back(center - halfUp + halfRight);

	//Print face vertices
	std::cout << "Face vertices: " << std::endl;
	for (const auto& v : face.verts) {
		std::cout << glm::to_string(v) << std::endl;
	}


    return face;
}

bool Collision::ClipPolygonAgainstPlane(std::vector<glm::vec3>& inputVerts, const glm::vec3& planeNormal, float planeDist, std::vector<glm::vec3>& outVerts)
{
	outVerts.clear();

    for (size_t i = 0; i < inputVerts.size(); ++i) {
        glm::vec3 a = inputVerts[i];
		glm::vec3 b = inputVerts[(i + 1) % inputVerts.size()];

		float da = glm::dot(planeNormal, a) - planeDist;
		float db = glm::dot(planeNormal, b) - planeDist;

		bool aInside = da <= 0;
		bool bInside = db <= 0;

        if(aInside && bInside) {
            outVerts.push_back(b);
        }
        else if(aInside && !bInside) {
            float t = da / (da - db);
            glm::vec3 intersection = a + t * (b - a);
            outVerts.push_back(intersection);
        }
        else if(!aInside && bInside) {
            float t = da / (da - db);
            glm::vec3 intersection = a + t * (b - a);
            outVerts.push_back(intersection);
            outVerts.push_back(b);
		}
    }

    return outVerts.size() >= 3;
}

std::vector<glm::vec3> Collision::ClipIncidentFaceToReferenceFace(const Face& incidentFace, const Face& referenceFace)
{
	std::vector<glm::vec3> clipped = incidentFace.verts;

    //Reference face center

	glm::vec3 faceCenter = glm::vec3(0.0f);

    for(const auto& v : referenceFace.verts)
        faceCenter += v;

	faceCenter /= static_cast<float>(referenceFace.verts.size());

	// 4 edge planes of the reference face

    for (int i = 0; i < 4; ++i) {
		glm::vec3 a = referenceFace.verts[i];
		glm::vec3 b = referenceFace.verts[(i + 1) % 4];
		glm::vec3 edge = b - a;
		glm::vec3 edgeNormal = glm::normalize(glm::cross(edge, referenceFace.normal));

		float planeDist = glm::dot(edgeNormal, a);

        std::vector<glm::vec3> out;
		ClipPolygonAgainstPlane(clipped, edgeNormal, planeDist, out);

        clipped = out;

        if (clipped.empty()) break;
    }

    return clipped;
}

void EnsureFaceWindingCorrect(Face& face) {
    // Hesapla: normal = cross((v1 - v0), (v2 - v0))
    glm::vec3 computedNormal = glm::normalize(glm::cross(face.verts[1] - face.verts[0], face.verts[2] - face.verts[0]));

    // Eðer normal ters yönde bakýyorsa, sýralamayý ters çevir
    if (glm::dot(computedNormal, face.normal) < 0.0f) {
        std::reverse(face.verts.begin(), face.verts.end());
    }
}

bool IsAxisParallelToOBBAxis(const glm::vec3& axis, const OBB& obb)
{
    for (int i = 0; i < 3; ++i) {
        float dot = fabs(glm::dot(glm::normalize(axis), obb.orientation[i]));
        if (fabs(dot - 1.0f) < 1e-3f) {
            return true;
        }
    }
    return false;
}

bool IsAxisCrossProduct(const glm::vec3& axis, const OBB& obbA, const OBB& obbB)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            glm::vec3 cross = glm::cross(obbA.orientation[i], obbB.orientation[j]);
			if (glm::length(cross) < 1e-3f) continue;
			cross = glm::normalize(cross);
            float dot = fabs(glm::dot(cross, glm::normalize(axis)));
            if (fabs(dot) > 1.0f - 1e-3f) {
                return true;
            }
        }
    }
    return false;
}

glm::vec3 GetSupportPoint(const OBB& obb, const glm::vec3& dir) {
	glm::vec3 result = obb.center;

    for (int i = 0; i < 3; ++i) {
		float sign = (glm::dot(dir, obb.orientation[i]) > 0.0f) ? 1.0f : -1.0f;
		result += obb.orientation[i] * obb.halfExtents[i] * sign;
    }

	return result;
}

glm::vec3 GetDeepestPoint(const OBB& obbA, const OBB& obbB, const glm::vec3& normal) {
	float bestDepth = -FLT_MAX;
    glm::vec3 bestVertex;

    for(int x = -1; x <= 1; x += 2) {
        for(int y = -1; y <= 1; y += 2) {
            for(int z = -1; z <= 1; z += 2) {
                glm::vec3 corner = obbA.center
                    + obbA.orientation[0] * obbA.halfExtents.x * (float)x
                    + obbA.orientation[1] * obbA.halfExtents.y * (float)y
					+ obbA.orientation[2] * obbA.halfExtents.z * (float)z;
                float depth = glm::dot(normal, obbB.center - corner);
                if (depth > bestDepth) {
                    bestDepth = depth;
                    bestVertex = corner;
                }
            }
        }
	}
	return bestVertex;
}
std::vector<std::pair<glm::vec3, glm::vec3>> GetEdgeCandidatesFromOBB(const OBB& obb, const glm::vec3& axis)
{
    std::vector<std::pair<glm::vec3, glm::vec3>> result;
    float maxDot = -FLT_MAX;
    int bestAxisIndex = -1;

    // En paralel olan ekseni bul
    for (int i = 0; i < 3; ++i) {
        float dot = fabs(glm::dot(obb.orientation[i], axis));
        if (dot > maxDot) {
            maxDot = dot;
            bestAxisIndex = i;
        }
    }

    glm::vec3 dir = obb.orientation[bestAxisIndex]; // en paralel kenar yönü
    glm::vec3 up = obb.orientation[(bestAxisIndex + 1) % 3];
    glm::vec3 right = obb.orientation[(bestAxisIndex + 2) % 3];

    float h0 = obb.halfExtents[(bestAxisIndex + 1) % 3];
    float h1 = obb.halfExtents[(bestAxisIndex + 2) % 3];
    glm::vec3 center = obb.center;

    // 4 kenarý oluþtur (bu eksen boyunca)
    for (int y = -1; y <= 1; y += 2) {
        for (int z = -1; z <= 1; z += 2) {
            glm::vec3 offset = up * (float)y * h0 + right * (float)z * h1;
            glm::vec3 start = center + offset - dir * obb.halfExtents[bestAxisIndex];
            glm::vec3 end = center + offset + dir * obb.halfExtents[bestAxisIndex];
            result.emplace_back(start, end);
        }
    }

    return result;
}

void Collision::AddEdgeFaceContacts(const glm::vec3& edgeStart, const glm::vec3& edgeEnd,
    const Face& referenceFace,
    const glm::vec3& collisionNormal,
    CollisionManifold& manifold)
{
    std::vector<glm::vec3> clipped = { edgeStart, edgeEnd };

    for (int i = 0; i < 4; ++i) {
        glm::vec3 a = referenceFace.verts[i];
        glm::vec3 b = referenceFace.verts[(i + 1) % 4];
        glm::vec3 edge = b - a;
        glm::vec3 edgeNormal = glm::normalize(glm::cross(edge, referenceFace.normal));
        float planeDist = glm::dot(edgeNormal, a);

        std::vector<glm::vec3> out;
        ClipPolygonAgainstPlane(clipped, edgeNormal, planeDist, out);
        clipped = out;
        if (clipped.empty()) return;
    }

    for (const auto& p : clipped) {
        CollisionContact contact;
        contact.contactPoint = p;
        contact.normal = collisionNormal;
        float depth = glm::dot(referenceFace.normal, referenceFace.verts[0] - p);
        contact.penetrationDepth = glm::max(0.0f, depth);
        manifold.contacts.push_back(contact);
    }
}

void ReduceContactPoints(std::vector<CollisionContact>& contacts, size_t maxContacts = 2)
{
    if (contacts.size() <= maxContacts)
        return;

    // 1. En derin contact'ý bul
    auto maxIt = std::max_element(contacts.begin(), contacts.end(),
        [](const CollisionContact& a, const CollisionContact& b) {
            return a.penetrationDepth < b.penetrationDepth;
        });

    CollisionContact first = *maxIt;
    contacts.erase(maxIt);

    // 2. Ýlk contact'tan en uzak olaný bul
    auto farthestIt = std::max_element(contacts.begin(), contacts.end(),
        [&](const CollisionContact& a, const CollisionContact& b) {
            float da = glm::length2(a.contactPoint - first.contactPoint);
            float db = glm::length2(b.contactPoint - first.contactPoint);
            return da < db;
        });

    CollisionContact second = *farthestIt;

    // 3. Sadece bu ikisini tut
    contacts.clear();
    contacts.push_back(first);
    contacts.push_back(second);
}

bool PointFaceContact(const glm::vec3& point, const Face& face, CollisionContact& outContact)
{
    float planeDist = glm::dot(face.normal, face.verts[0]);
    float dist = glm::dot(face.normal, point) - planeDist;

    std::cout << "Point: " << glm::to_string(point) << ", dist to plane: " << dist << std::endl;
    
    if (dist > 0.0f)
        return false;

    glm::vec3 projectedPoint = point - dist * face.normal;

    // Polygon içinde mi diye kontrol et (face 4 köþeli)
    for (int i = 0; i < 4; ++i) {
        glm::vec3 a = face.verts[i];
        glm::vec3 b = face.verts[(i + 1) % 4];
        glm::vec3 edge = b - a;
        glm::vec3 edgeNormal = glm::normalize(glm::cross(face.normal, edge));

        if (glm::dot(projectedPoint - a, edgeNormal) > 0) {
            return false;
        }
    }

    outContact.contactPoint = projectedPoint;
    outContact.normal = face.normal;
    outContact.penetrationDepth = -dist;

    return true;
}

bool Collision::TestOBBOBB(const OBB& obbA, const OBB& obbB, CollisionManifold& outData)
{
    /* Test OBB vs OBB using Separating Axis Theorem(SAT) */
    glm::vec3 axes[15]{};
    axes[0] = obbA.orientation[0];
    axes[1] = obbA.orientation[1];
    axes[2] = obbA.orientation[2];

    axes[3] = obbB.orientation[0];
    axes[4] = obbB.orientation[1];
    axes[5] = obbB.orientation[2];

    // Cross products of edges
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            axes[6 + i * 3 + j] = glm::cross(obbA.orientation[i], obbB.orientation[j]);
        }
    }

    glm::vec3 toCenter = obbB.center - obbA.center;

    // Test all axes
    float minPenetration = FLT_MAX;
    glm::vec3 bestAxis;
    for (const auto& axis : axes) {
        float axisLen = glm::length(axis);
        if (axisLen < 1e-6f) continue;
        glm::vec3 normAxis = axis / axisLen;
        // Project both OBBs onto the axis
        float aMin, aMax, bMin, bMax;
        // Project OBB A
        float aCenter = glm::dot(obbA.center, normAxis);
        float aExtent = obbA.halfExtents.x * fabs(glm::dot(obbA.orientation[0], normAxis)) +
            obbA.halfExtents.y * fabs(glm::dot(obbA.orientation[1], normAxis)) +
            obbA.halfExtents.z * fabs(glm::dot(obbA.orientation[2], normAxis));
        aMin = aCenter - aExtent;
        aMax = aCenter + aExtent;
        // Project OBB B
        float bCenter = glm::dot(obbB.center, normAxis);
        float bExtent = obbB.halfExtents.x * fabs(glm::dot(obbB.orientation[0], normAxis)) +
            obbB.halfExtents.y * fabs(glm::dot(obbB.orientation[1], normAxis)) +
            obbB.halfExtents.z * fabs(glm::dot(obbB.orientation[2], normAxis));
        bMin = bCenter - bExtent;
        bMax = bCenter + bExtent;
        // Check for overlap
        if (aMax < bMin || bMax < aMin) {
            return false; // Found separating axis
        }
        // Calculate penetration depth
        float overlap = (std::min(aMax, bMax) - std::max(aMin, bMin)) / axisLen;
        if (overlap < minPenetration) {
            minPenetration = overlap;
            bestAxis = normAxis;
            // Ensure the normal points from A to B
            if (glm::dot(bestAxis, toCenter) < 0)
                bestAxis = -bestAxis;
        }
    }
    // If we reach here, no separating axis was found; the OBBs are colliding
    float penetrationDepth = minPenetration;
    glm::vec3 normal = glm::normalize(bestAxis);


    if (IsAxisParallelToOBBAxis(normal, obbA) && IsAxisParallelToOBBAxis(normal, obbB)) {
        std::cout << "Face-face contact detected." << std::endl;

        bool refFlip = false;
        int refIndex = GetReferenceFaceIndex(obbA, normal, refFlip);
        Face referenceFace = GetFace(obbA, refIndex, refFlip);
        EnsureFaceWindingCorrect(referenceFace);

        bool incFlip = false;
        int incIndex = GetReferenceFaceIndex(obbB, -normal, incFlip);
        Face incidentFace = GetFace(obbB, incIndex, incFlip);
        EnsureFaceWindingCorrect(incidentFace);

        // Clipping
        auto clippedPoints = ClipIncidentFaceToReferenceFace(incidentFace, referenceFace);

        CollisionManifold manifold;

        for (const auto& p : clippedPoints) {

            std::cout << "Clipped point: " << glm::to_string(p) << std::endl;

            CollisionContact contact;
            contact.contactPoint = p;
            contact.normal = normal;
            float depth = glm::dot(referenceFace.normal, referenceFace.verts[0] - p);
            contact.penetrationDepth = glm::max(0.0f, depth);

            manifold.contacts.push_back(contact);

        }
        outData = manifold;
    }
    else if(IsAxisCrossProduct(normal, obbA, obbB)) {
		std::cout << "Edge-edge contact detected." << std::endl;

        // 1. Referans yüzü A’dan seçelim (ayný mantýkla B'den de olabilir)
        bool refFlip = false;
        int refIndex = GetReferenceFaceIndex(obbA, normal, refFlip);
        Face referenceFace = GetFace(obbA, refIndex, refFlip);
        EnsureFaceWindingCorrect(referenceFace);

        // 2. OBB B’den bu eksene en paralel olan kenarlarý al
        auto edgeCandidates = GetEdgeCandidatesFromOBB(obbB, -normal); // normal ters, çünkü B çarpýyor

        CollisionManifold manifold;

        // 3. Her kenarý yüzle clip’le
        for (const auto& [start, end] : edgeCandidates) {
            AddEdgeFaceContacts(start, end, referenceFace, normal, manifold);
        }

		ReduceContactPoints(manifold.contacts, 2);

        outData = manifold;
    }
    else {
        CollisionManifold manifold;

        bool refFlip = false;
        int refIndex = GetReferenceFaceIndex(obbA, normal, refFlip);
        Face referenceFace = GetFace(obbA, refIndex, refFlip);
        EnsureFaceWindingCorrect(referenceFace);

        bool contactFound = false;
        for (int x = -1; x <= 1; x += 2) {
            for (int y = -1; y <= 1; y += 2) {
                for (int z = -1; z <= 1; z += 2) {
                    glm::vec3 corner = obbB.center
                        + obbB.orientation[0] * obbB.halfExtents.x * (float)x
                        + obbB.orientation[1] * obbB.halfExtents.y * (float)y
                        + obbB.orientation[2] * obbB.halfExtents.z * (float)z;

                    CollisionContact contact;
                    if (PointFaceContact(corner, referenceFace, contact)) {
                        contact.normal = normal;
                        manifold.contacts.push_back(contact);
                        contactFound = true;

                        std::cout << "Contact point found: " << glm::to_string(contact.contactPoint) << std::endl;
                    }
                }
            }
        }

        if (contactFound) {
            outData = manifold;
            return true;
        }""
    }


    /*else {
        // Temporary fallback: single contact at the center between OBBs
        CollisionContact contact;

        glm::vec3 projA = obbA.center - referenceFace.normal * glm::dot(referenceFace.normal, obbA.center - referenceFace.verts[0]);
        glm::vec3 projB = obbB.center - (-referenceFace.normal) * glm::dot(-referenceFace.normal, obbB.center - referenceFace.verts[0]);
        contact.contactPoint = (glm::length(projA - obbA.center) < glm::length(projB - obbB.center)) ? projA : projB;
        contact.normal = normal;
        contact.penetrationDepth = penetrationDepth;
        //outData.contacts.push_back(contact);
    }*/

    std::cout << "Contact points found: " << outData.contacts.size() << std::endl;

    return true;
}
#endif