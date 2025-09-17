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
    
	// Relative velocity
    glm::vec3 rA = data.contactPoint - obbA.center;
    glm::vec3 rB = data.contactPoint - obbB.center;

	glm::vec3 vA = A->velocity + glm::cross(A->angularVelocity, rA);
	glm::vec3 vB = B->velocity + glm::cross(B->angularVelocity, rB);
    glm::vec3 relVel = vB - vA;

	std::cout << "Relative Velocity: " << glm::to_string(relVel) << std::endl;

    // Relative velocity along the normal
    float velAlongNormal = glm::dot(relVel, data.normal);
    if (velAlongNormal > 0) return; // They are separating

    //Calculate inertia tensors in world space
    glm::vec3 fullExtentsA = obbA.halfExtents * 2.0f;
    glm::vec3 inertiaA = (A->GetMass() / 12.0f) * glm::vec3(
        fullExtentsA.y * fullExtentsA.y + fullExtentsA.z * fullExtentsA.z,
        fullExtentsA.x * fullExtentsA.x + fullExtentsA.z * fullExtentsA.z,
        fullExtentsA.x * fullExtentsA.x + fullExtentsA.y * fullExtentsA.y
    );
    A->invInertiaDiag = glm::vec3(
        inertiaA.x > 0.0f ? 1.0f / inertiaA.x : 0.0f,
        inertiaA.y > 0.0f ? 1.0f / inertiaA.y : 0.0f,
        inertiaA.z > 0.0f ? 1.0f / inertiaA.z : 0.0f
    );

    glm::vec3 fullExtentsB = obbB.halfExtents * 2.0f;
    glm::vec3 inertiaB = (B->GetMass() / 12.0f) * glm::vec3(
        fullExtentsB.y * fullExtentsB.y + fullExtentsB.z * fullExtentsB.z,
        fullExtentsB.x * fullExtentsB.x + fullExtentsB.z * fullExtentsB.z,
        fullExtentsB.x * fullExtentsB.x + fullExtentsB.y * fullExtentsB.y
    );
    B->invInertiaDiag = glm::vec3(
        inertiaB.x > 0.0f ? 1.0f / inertiaB.x : 0.0f,
        inertiaB.y > 0.0f ? 1.0f / inertiaB.y : 0.0f,
        inertiaB.z > 0.0f ? 1.0f / inertiaB.z : 0.0f
    );

    //Calculate inverted inertia tensors in world space
    glm::mat3 invInertiaA = obbA.orientation * glm::mat3(
        A->invInertiaDiag.x, 0.0f, 0.0f,
        0.0f, A->invInertiaDiag.y, 0.0f,
        0.0f, 0.0f, A->invInertiaDiag.z
    ) * glm::transpose(obbA.orientation);

    glm::mat3 invInertiaB = obbB.orientation * glm::mat3(
        B->invInertiaDiag.x, 0.0f, 0.0f,
        0.0f, B->invInertiaDiag.y, 0.0f,
        0.0f, 0.0f, B->invInertiaDiag.z
    ) * glm::transpose(obbB.orientation);

	std::cout << "Inertia A: " << glm::to_string(inertiaA) << ", Inertia B: " << glm::to_string(inertiaB) << std::endl;

    /* LINEAR VELOCITY */

	// Precompute cross products
    glm::vec3 rAxN = glm::cross(rA, data.normal);
    glm::vec3 rBxN = glm::cross(rB, data.normal);

	//Calculate denominator for impulse scalar
	float denom = A->GetInvMass() + B->GetInvMass() + glm::dot(data.normal, glm::cross(invInertiaA * rAxN, rA) + glm::cross(invInertiaB * rBxN, rB));

	// Calculate impulse scalar
	float j = -(1.0f + restitution) * velAlongNormal / denom;

    // Apply impulse
    glm::vec3 impulse = j * data.normal;

    /* ANGULAR VELOCITY */

	// Calculate change in angular velocity
    glm::vec3 deltaAngularVelA = invInertiaA * glm::cross(rA, -impulse);
	glm::vec3 deltaAngularVelB = invInertiaB * glm::cross(rB, impulse);

	// Apply impulse to linear and angular velocities
    if (A->IsPhysical()) {
        A->velocity -= impulse * A->GetInvMass();
		A->angularVelocity += deltaAngularVelA;
    }
    if (B->IsPhysical()) {
        B->velocity += impulse * B->GetInvMass();
		B->angularVelocity += deltaAngularVelB;
    }

    /* FRICTION */

    glm::vec3 tangent = relVel - (velAlongNormal * data.normal);
	float tangentMag = glm::length(tangent);

    if (tangentMag > 0.0000001f) {
        tangent = glm::normalize(tangent);
    }
    else {
		return; // No tangential velocity, so no friction
    }
    
	glm::vec3 rAxT = glm::cross(rA, tangent);
	glm::vec3 rBxT = glm::cross(rB, tangent);

    denom = A->GetInvMass() + B->GetInvMass() +
		glm::dot(tangent, glm::cross(invInertiaA * rAxT, rA) + glm::cross(invInertiaB * rBxT, rB));

    float jt = -glm::dot(relVel, tangent) / denom;

	float mu = frictionCoeff; // Coefficient of friction
	float jn = j; // Normal impulse magnitude

	jt = glm::clamp(jt, -mu * jn, mu * jn); // Coulomb's law
    glm::vec3 frictionImpulse = jt * tangent;

    // Apply friction impulse
    if (A->IsPhysical()) {
        A->velocity -= frictionImpulse * A->GetInvMass();
        A->angularVelocity += invInertiaA * glm::cross(rA, frictionImpulse);
    }
    if (B->IsPhysical()) {
        B->velocity += frictionImpulse * B->GetInvMass();
        B->angularVelocity += invInertiaB * glm::cross(rB, -frictionImpulse);
	}

	std::cout << "Impulse applied: " << glm::to_string(impulse) << ", Friction impulse: " << glm::to_string(frictionImpulse) << std::endl;
}

void Collision::PositionalCorrection(Physical* A, Physical* B, glm::vec3& posA, glm::vec3& posB, const CollisionData& data, float percent, float slop)
{
    float correctionMag = glm::max(data.penetrationDepth - slop, 0.0f) / (A->GetInvMass() + B->GetInvMass()) * percent;
    glm::vec3 correction = correctionMag * data.normal;

    if (A->IsPhysical())
        posA -= correction * A->GetInvMass();
    if (B->IsPhysical())
        posB += correction * B->GetInvMass();
}

bool Collision::TestOBBOBB(const OBB& obbA, const OBB& obbB, CollisionData& outData)
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
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            axes[6 + i * 3 + j] = glm::cross(obbA.orientation[i], obbB.orientation[j]);
        }
	}

	glm::vec3 toCenter = obbB.center - obbA.center;

    // Test all axes
    float minPenetration = FLT_MAX;
    glm::vec3 bestAxis;
    for (const auto& axis : axes) {
        if (glm::length(axis) < 1e-6) continue; // Skip near-zero axes
        glm::vec3 normAxis = glm::normalize(axis);
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
        float overlap = std::min(aMax, bMax) - std::max(aMin, bMin);
        if (overlap < minPenetration) {
            minPenetration = overlap;
            bestAxis = normAxis;
            // Ensure the normal points from A to B
            if (glm::dot(bestAxis, toCenter) < 0)
                bestAxis = -bestAxis;
        }
    }
    // If we reach here, no separating axis was found; the OBBs are colliding
    outData.penetrationDepth = minPenetration;
	outData.normal = glm::normalize(bestAxis);
    outData.contactPoint = (obbA.center + obbB.center) / 2.0f; // Approximate contact point

    return true;
}
