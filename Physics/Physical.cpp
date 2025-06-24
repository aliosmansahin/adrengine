#include "pch.h"
#include "Physical.h"

/*
PURPOSE: Calculates physics affects
*/
void Physical::Update(float deltaTime)
{
    if (!physicsEffects || mass <= 0.0f)
        return;

    // Velocity from impulse
    velocity += impulse / mass;

    // Acceleration
    glm::vec3 acceleration = force / mass;
    velocity += acceleration * deltaTime;

    // Friction and damping
    float normalForce = mass * 9.81f;
    float frictionMagnitude = frictionCoefficient * normalForce;

    //Temporary friction calculations
    if (pow(glm::length(velocity), 2) > 1e-6f) {
        glm::vec3 frictionDir = -glm::normalize(velocity);
        glm::vec3 frictionForce = frictionDir * frictionMagnitude;

        glm::vec3 velocityChange = (frictionForce / mass) * deltaTime;

        if (pow(glm::length(velocityChange), 2) > pow(glm::length(velocity), 2)) {
            velocity = glm::vec3(0.0f);
        }
        else {
            velocity += velocityChange;
        }

        // Damping
        velocity *= pow(1.0f - linearDamping, deltaTime);
    }

    // Stabilize tiny velocities to zero
    if (pow(glm::length(velocity), 2) < 1e-4f) {
        velocity = glm::vec3(0.0f);
    }

    // Reset forces
    force = glm::vec3(0.0f);
    impulse = glm::vec3(0.0f);
}

/*
PURPOSE: Adds a new force
*/
void Physical::ApplyForce(glm::vec3 force)
{
	this->force += force;
}

/*
PURPOSE: Adds a new impulse
*/
PHYSICS_API void Physical::ApplyImpulse(glm::vec3 force)
{
	this->impulse = force;
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
