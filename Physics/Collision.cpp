#include "pch.h"
#include "Collision.h"

// Collision impulse resolution
void Collision::ResolveCollisionImpulse(
    Physical* A, Physical* B,
    CollisionData& data,
	OBB& obbA, OBB& obbB,
    double restitution,
    double frictionCoeff
) {
    if (!A->IsPhysical() && !B->IsPhysical()) return;

    glm::vec3 rA = data.contactPoint - obbA.center;
    glm::vec3 rB = data.contactPoint - obbB.center;

    glm::vec3 vAcontact = A->velocity + glm::cross(A->angularVelocity, rA);
    glm::vec3 vBcontact = B->velocity + glm::cross(B->angularVelocity, rB);
    glm::vec3 relVel = vBcontact - vAcontact;

    float relNormalVel = glm::dot(relVel, data.normal);
    if (relNormalVel > 0.0) return; // separating

    // if bodies are essentially resting contact, disable restitution to avoid bounce
    const float REST_VEL_THRESHOLD = 0.25f; // tweak: 0.1..0.5 typical
    float e = (std::abs(relNormalVel) < REST_VEL_THRESHOLD) ? 0.0f : restitution;

    // compute effective mass (denominator)
    float invMassSum = A->GetInvMass() + B->GetInvMass();

    glm::vec3 rAxn = glm::cross(rA, data.normal);
    glm::vec3 rBxn = glm::cross(rB, data.normal);

    float rotTermA = glm::dot(rAxn * A->invInertiaDiag, rAxn);
    float rotTermB = glm::dot(rBxn * B->invInertiaDiag, rBxn);

    float denom = invMassSum + rotTermA + rotTermB;
    if (denom <= 1e-12f) return;

	//Normal impulse scalar
    float j = -(1.0 + e) * relNormalVel / denom;

    //If one is static, don't scale by its invMass (we already have invMassSum; impulse application below uses invMass)
    glm::vec3 impulse = data.normal * j;

    //Apply normal impulse
    if (A->IsPhysical()) {
        A->velocity -= impulse * A->GetInvMass();
        glm::vec3 dWA = glm::cross(rA, impulse);
        A->angularVelocity += dWA * A->invInertiaDiag;
    }
    if (B->IsPhysical()) {
        B->velocity += impulse * B->GetInvMass();
        glm::vec3 dWB = glm::cross(rB, impulse);
        B->angularVelocity -= dWB * B->invInertiaDiag;
    }

    // --- Friction ---
    vAcontact = A->velocity + glm::cross(A->angularVelocity, rA);
    vBcontact = B->velocity + glm::cross(B->angularVelocity, rB);
    relVel = vBcontact - vAcontact;

    glm::vec3 tangent = relVel - data.normal * glm::dot(relVel, data.normal);
    float tLen2 = glm::length2(tangent);
    if (tLen2 <= 1e-12) return;
    tangent = glm::normalize(tangent);

	//effective mass along tangent
    glm::vec3 rAxT = glm::cross(rA, tangent);
    glm::vec3 rBxT = glm::cross(rB, tangent);

    float rotTermAT = glm::dot(rAxT * A->invInertiaDiag, rAxT);
    float rotTermBT = glm::dot(rBxT * B->invInertiaDiag, rBxT);

    float denomT = invMassSum + rotTermAT + rotTermBT;
    if (denomT <= 1e-12f) return;

	float relTangentVel = glm::dot(relVel, tangent);
    float jt = -relTangentVel / denomT;

	//Coulomb friction clamp |jt| <= mu * j
    float maxFriction = frictionCoeff * j;
    jt = glm::clamp(jt, -maxFriction, maxFriction);

    glm::vec3 frictionImpulse = tangent * jt;

	//Apply friction impulse
    if (A->IsPhysical()) {
        A->velocity -= frictionImpulse * A->GetInvMass();
        glm::vec3 dFA = glm::cross(rA, frictionImpulse);
        A->angularVelocity += dFA * A->invInertiaDiag;
    }
    if (B->IsPhysical()) {
        B->velocity += frictionImpulse * B->GetInvMass();
        glm::vec3 dFB = glm::cross(rB, frictionImpulse);
        B->angularVelocity -= dFB * B->invInertiaDiag;
    }

	// Positional correction to avoid sinking
	//PositionalCorrection(A, B, data);
}

void Collision::ProjectOntoAxis(const OBB& box, const glm::vec3& axis, float& outMin, float& outMax)
{
	glm::vec3 absAxis = glm::abs(axis);
    float radius =  box.halfExtents.x * glm::dot(absAxis, box.orientation[0]) +
                    box.halfExtents.y * glm::dot(absAxis, box.orientation[1]) +
		            box.halfExtents.z * glm::dot(absAxis, box.orientation[2]);
	float boxCenterProj = glm::dot(axis, box.center);
	outMin = boxCenterProj - radius;
	outMax = boxCenterProj + radius;
}

float Collision::OBBProjectedRadius(const OBB& box, const glm::vec3& axis)
{
    glm::vec3 a0 = box.orientation[0];
	glm::vec3 a1 = box.orientation[1];
	glm::vec3 a2 = box.orientation[2];
    return  box.halfExtents.x * glm::abs(glm::dot(axis, a0)) +
            box.halfExtents.y * glm::abs(glm::dot(axis, a1)) +
		    box.halfExtents.z * glm::abs(glm::dot(axis, a2));
}

void Collision::PositionalCorrection(Physical* A, Physical* B, const CollisionData& data, float percent, float slop)
{
	float correctionDepth = glm::max(data.penetrationDepth - slop, 0.0f);

	float invMassSum = A->GetInvMass() + B->GetInvMass();

	if (invMassSum <= 0.0f) return;

	glm::vec3 correction = (correctionDepth / invMassSum) * data.normal * percent;
    if(A->IsPhysical())
		A->velocity -= correction * A->GetInvMass(); // Simple positional correction via velocity change
	if (B->IsPhysical())
		B->velocity += correction * B->GetInvMass();
}

bool Collision::TestOBBOBB(const OBB& obbA, const OBB& obbB, CollisionData& outData)
{
	const float EPS_SKIP = 1e-6f;

	// 15 potential separating axes:
    glm::vec3 axes[15];
	axes[0] = obbA.orientation[0];
	axes[1] = obbA.orientation[1];
	axes[2] = obbA.orientation[2];
    
	axes[3] = obbB.orientation[0];
	axes[4] = obbB.orientation[1];
	axes[5] = obbB.orientation[2];

	// Cross products of edges
    int idx = 6;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            axes[idx] = glm::cross(obbA.orientation[i], obbB.orientation[j]);
        }
	}

	float minOverlap = FLT_MAX;
	glm::vec3 bestAxis(1.0f, 0.0f, 0.0f);
    float bestAstore = 0.0f;
	float bestBstore = 0.0f;

	glm::vec3 centerDiff = obbB.center - obbA.center;

    for (int i = 0; i < 15; ++i) {
        glm::vec3 axis = axes[i];

		//skip near-zero axes (possible for parallel axes cross-products)
        float axisLen2 = glm::length2(axis);
        if (axisLen2 < EPS_SKIP) continue; // Skip near-zero axes

		axis = glm::normalize(axis);

		//projected distance between centers onto axis (signed)
		float dist = glm::dot(centerDiff, axis);
		float distAbs = glm::abs(dist);
        
		//projected radius of each box onto axis
		float rA = OBBProjectedRadius(obbA, axis);
		float rB = OBBProjectedRadius(obbB, axis);

		float overlap = rA + rB - distAbs;
        if (overlap < 0.0f) {
            return false; // Found a separating axis
        }

        if (overlap < minOverlap) {
            minOverlap = overlap;
            bestAxis = axis;
            if (dist < 0.0f) bestAxis = -bestAxis; // Ensure normal points from A to B
            bestAstore = rA;
			bestBstore = rB;
		}
    }

	glm::vec3 pointA = obbA.center + bestAxis * bestAstore;
	glm::vec3 pointB = obbB.center - bestAxis * bestBstore;

	outData.contactPoint = (pointA + pointB) * 0.5f; // Approximate contact point
	outData.normal = glm::normalize(bestAxis); // Normal from A to B
	outData.penetrationDepth = minOverlap; // Penetration depth calculation can be added if needed
    
	return true; // No separating axis found, boxes must be intersecting
}
