#include "pch.h"
#include "Physical.h"

/*
PURPOSE: Calculates physics affects
*/
void Physical::Update(float deltaTime, glm::vec3& pos, glm::vec3& rot)
{
	//TODO: Update all physics here
}

/*
PURPOSE: Adds a new force
*/
void Physical::ApplyForce(glm::vec3 force)
{
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