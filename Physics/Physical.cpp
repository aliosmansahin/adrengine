#include "pch.h"
#include "Physical.h"

//INITIALIZE STATIC MEMBERS
float Physical::gravity = 9.81f;

/*
PURPOSE: Calculates physics effects
*/
void Physical::Update(float deltaTime, glm::vec3& pos, glm::vec3& rot)
{
	/* Update all physics here */
	//TODO: DeltaTime effects on physics calculations like velocity
	
	
	//Damping
	//TODO: Add surface size to calculate damping more accurately

    glm::vec3 frictionForce = this->friction * glm::vec3(
        glm::pow(velocity.x, 2.0f),
        glm::pow(velocity.y, 2.0f),
        glm::pow(velocity.z, 2.0f)
    );
	if (glm::length(velocity) < 0.5f) velocity = glm::vec3(0.0f);

	force -= frictionForce;

	//Add gravity force
	if (enableGravity) {
		force += glm::vec3(0.0f, -gravity * mass, 0.0f);
	}

	//Calculate acceleration
	glm::vec3 acceleration = force / mass;
	velocity += acceleration * deltaTime * 0.5f;
	pos += velocity * deltaTime;
	velocity += acceleration * deltaTime * 0.5f;

	//Reset force
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