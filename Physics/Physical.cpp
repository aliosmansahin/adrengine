#include "pch.h"
#include "Physical.h"

/*
PURPOSE: Calculates physics affects
*/
void Physical::Update(float deltaTime)
{
	if (!physicsEffects || mass <= 0.0f)
		return;

	velocity += impulse / mass;

	glm::vec3 acceleration = force / mass;

	velocity += acceleration * deltaTime;

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
