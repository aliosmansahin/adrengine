#include "pch.h"
#include "Physical.h"

//INITIALIZE STATIC MEMBERS
glm::vec3 Physical::gravity = glm::vec3(0.0f, -9.81f, 0.0f);

void Physical::IntegrateForcesAndVelocities(float dt, glm::vec3& pos)
{
    if (!IsPhysical()) return;

    // Linear acceleration
    glm::vec3 accel = accumulatedForce * invMass;

	if (enableGravity)
        accel += gravity;

    // Velocity update
    velocity += accel * dt;

    // Angular acceleration
    glm::vec3 alpha = glm::vec3(
        invInertiaDiag.x * accumulatedTorque.x,
        invInertiaDiag.y * accumulatedTorque.y,
        invInertiaDiag.z * accumulatedTorque.z
    );
    angularVelocity += alpha * dt;

    // Damping
    double ld = glm::clamp(1.0 - linearDamping * dt, 0.0, 1.0);
    double ad = glm::clamp(1.0 - angularDamping * dt, 0.0, 1.0);
    velocity *= ld;
    angularVelocity *= ad;

    // Position update
    pos += velocity * dt;

    // Clear forces for next frame
    ClearForces();
}
