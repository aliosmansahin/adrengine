#include "pch.h"
#include "Physical.h"

/*
PURPOSE: Calculates physics affects
*/
void Physical::Update(float deltaTime, glm::vec3& pos, glm::vec3& rot)
{
    //physicsEffects have to be true to update physics
    if (!physicsEffects || mass <= 0.0f)
        return;

    //Frictions
    /*velocity *= std::max(0.0f, 1.0f - linearDamping * deltaTime);
    angularVelocity *= std::max(0.0f, 1.0f - angularDamping * deltaTime);*/

    glm::vec3 frictionAccel = glm::vec3(0.0f);

    if (glm::length(velocity) > 1e-4f) {
        glm::vec3 frictionDir = -glm::normalize(velocity);
        float invMass = !IsPhysical() ? 0.0f : 1.0f / mass;
        glm::vec3 frictionForce = friction * force * frictionDir;
        frictionAccel = frictionForce * invMass;
    }

    //force -> acceleration -> velocity -> position
    glm::vec3 acceleration = force / mass;
    velocity += (acceleration + frictionAccel) * deltaTime * 0.5f;
    pos += velocity * deltaTime;
    velocity += (acceleration + frictionAccel) * deltaTime * 0.5f;

    if (glm::length(velocity) < 1e-3f)
        velocity = glm::vec3(0.0f);

    //angularVelocity -> rotation
    float angle = glm::length(angularVelocity);

    if (angle > 0.0001f) {

        glm::vec3 axis = glm::normalize(angularVelocity);

        glm::quat deltaRotation = glm::angleAxis(angle * deltaTime, axis);

        glm::quat currentRotationQuat = glm::quat(glm::radians(rot));

        currentRotationQuat = glm::normalize(deltaRotation * currentRotationQuat);

        glm::vec3 newEuler = glm::degrees(glm::eulerAngles(currentRotationQuat));

        rot = newEuler;
    }

    // Reset forces
    force = glm::vec3(0.0f);
}

/*
PURPOSE: Adds a new force
*/
void Physical::ApplyForce(glm::vec3 force)
{
	this->force += force;
}

/*
PURPOSE: Returns if this instance is physical
*/
bool Physical::IsPhysical()
{
	return physicsEffects;
}

/*
PURPOSE: Sets physical state of this instance
*/
void Physical::SetPhysical(bool physical)
{
	physicsEffects = physical;
}

/*
PURPOSE: Returns the velocity of this instance
*/
PHYSICS_API glm::vec3 Physical::GetVelocity()
{
	return velocity;
}

/*
PURPOSE: Sets velocity
*/
PHYSICS_API void Physical::SetVelocity(glm::vec3 velo)
{
    velocity = velo;
}

/*
PURPOSE: Resets variables
*/
PHYSICS_API void Physical::Reset()
{
    velocity = glm::vec3(0.0f);
    angularVelocity = glm::vec3(0.0f);
    force = glm::vec3(0.0f);
}