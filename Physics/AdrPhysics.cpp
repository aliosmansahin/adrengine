#include "pch.h"
#include "AdrPhysics.h"
#include <glm/gtx/string_cast.hpp>

//INITIALIZE STATIC MEMBERS
glm::vec3 AdrPhysics::gravity = glm::vec3(0.0f, -9.81f, 0.0f);

float NormalizeAngle(float angle) {
    while (angle > 180.0f) angle -= 180;
    while (angle < -180.0f) angle += 180.0f;
    return angle;
}


void AdrPhysics::IntegrateForcesAndVelocities(float dt, glm::vec3& pos, glm::vec3& rot)
{
    enableGravity = true;
    if (!IsPhysical()) return;

    // Damping
    float ld = glm::clamp(1.0f - linearDamping * dt, 0.0f, 1.0f);
    float ad = glm::clamp(1.0f - angularDamping * dt, 0.0f, 1.0f);
    //velocity *= ld;
    //angularVelocity *= ad;

    // Discard very small velocities
    //if (glm::length(velocity) < 0.001f) velocity = glm::vec3(0.0f);
    //if (glm::length(angularVelocity) < 0.00000001f) angularVelocity = glm::vec3(0.0f);

    // Velocity update
    velocity += gravity * dt;

    // Position update
    pos += velocity * dt;

    rot += glm::degrees(angularVelocity) * dt;

    glm::quat currentRotationQuat = glm::quat(glm::radians(rot)); // Mevcut rotasyonu al
    glm::quat deltaRot;


    if (glm::length2(angularVelocity) > 0.0f) {
        glm::vec3 axis = glm::normalize(angularVelocity);
        float angle = glm::length(angularVelocity) * dt;
        deltaRot = glm::angleAxis(angle, axis);
        currentRotationQuat = deltaRot * currentRotationQuat;
    }

    glm::vec3 newEuler = glm::degrees(glm::eulerAngles(currentRotationQuat));

    for (int i = 0; i < 3; ++i) {
        float delta = newEuler[i] - rot[i];

        if (delta > 180.0f)       newEuler[i] -= 360.0f;
        else if (delta < -180.0f) newEuler[i] += 360.0f;
    }

    //std::cout << glm::to_string(angularVelocity) << " <<< angularVelo" << std::endl;



    // Clear forces for next frame
    ClearForces();
}
