#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

class AdrPhysics //TODO: Derive from a base Physics class
{
public:
    float mass = 1.0f;          // inverse mass (0 => immovable)
    glm::vec3 invInertiaDiag = glm::vec3(1.0f); // inverse inertia diagonal (body-space approx)
    glm::vec3 velocity;         // linear velocity
    glm::vec3 angularVelocity;  // angular velocity
    glm::vec3 accumulatedForce;
    glm::vec3 accumulatedTorque;
    float linearDamping = 0.6f;
    float angularDamping = 0.6f;
    static glm::vec3 gravity; // global gravity

    bool isPhysical = true;
    bool enableGravity = true;

    PHYSICS_API bool IsPhysical() const { return isPhysical && mass != 0.0f; }
    PHYSICS_API float GetInvMass() { if (mass <= 0.0f || !isPhysical) return 0.0f; return 1.0f / mass; }
    PHYSICS_API float GetMass() { if (!isPhysical) return 0.0f; return mass; }

    PHYSICS_API void ApplyForce(const glm::vec3& f) { accumulatedForce += f; }
    PHYSICS_API void ApplyTorque(const glm::vec3& t) { accumulatedTorque += t; }
    PHYSICS_API void ClearForces() { accumulatedForce = glm::vec3(0); accumulatedTorque = glm::vec3(0); }
    PHYSICS_API void Reset() {
        velocity = glm::vec3(0.0);
        angularVelocity = glm::vec3(0.0);
        ClearForces();
    }

    PHYSICS_API void IntegrateForcesAndVelocities(float dt, glm::vec3& pos, glm::vec3& rot);
};