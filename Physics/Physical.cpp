#include "pch.h"
#include "Physical.h"

//INITIALIZE STATIC MEMBERS
glm::vec3 Physical::gravity = glm::vec3(0.0f, -9.81f, 0.0f);

void Physical::IntegrateForcesAndVelocities(float dt, glm::vec3& pos, glm::vec3& rot)
{
    if (!IsPhysical()) return;

    // Linear acceleration
    glm::vec3 accel = accumulatedForce / mass;

	if (enableGravity)
        accel += gravity;

    // Velocity update
    velocity += accel * dt;

    // Damping
    float ld = glm::clamp(1.0f - linearDamping * dt, 0.0f, 1.0f);
    float ad = glm::clamp(1.0f - angularDamping * dt, 0.0f, 1.0f);
    velocity *= ld;
    angularVelocity *= ad;

	// Discard very small velocities
	if (glm::length(velocity) < 0.001f) velocity = glm::vec3(0.0f);
    if (glm::length(angularVelocity) < 0.001f) angularVelocity = glm::vec3(0.0f);

    // Position update
    pos += velocity * dt;
    rot += angularVelocity * dt;

    // Clear forces for next frame
    ClearForces();
}
