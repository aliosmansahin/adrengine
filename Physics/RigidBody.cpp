#include "pch.h"
#include "RigidBody.h"

/*

PURPOSE: Creates a RigidBody object and returns it

*/
btRigidBody* RigidBody::Create()
{
	shape = std::make_shared<btBoxShape>(btVector3(1, 1, 1));

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

	props = new RigidBodyProperties();
	props->shapeProps.halfExtentsForBox = glm::vec3(1.0f);

	btVector3 inertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);
	shape->calculateLocalInertia(props->mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(props->mass, motionState, shape.get(), inertia);

	rigidBody = std::make_shared<btRigidBody>(fallRigidBodyCI);

	rigidBody->setRestitution(props->restitution);

	rigidBody->setActivationState(DISABLE_DEACTIVATION);

	return rigidBody.get();
}

/*

PURPOSE: Clears unnecessary data

*/
void RigidBody::Delete()
{
	if (motionState)
		delete motionState;
	if (props)
		delete props;
}

/*

PURPOSE: Returns an enum class value to determine shape type
	BOX_SHAPE_PROXYTYPE -> Box
	No one -> None

*/
RigidBodyShape RigidBody::GetShape()
{
	switch (rigidBody->getCollisionShape()->getShapeType())
	{
	case BOX_SHAPE_PROXYTYPE:
		return RigidBodyShape::Box;
	default:
		return RigidBodyShape::None;
	}
}

/*

PURPOSE: Updates half extents

*/
PHYSICS_API void RigidBody::UpdateShapeHalfExtents(RigidBodyShape rbShape)
{
	//Update shape
	switch (rbShape)
	{
	case RigidBodyShape::Box: {
		btVector3 halfExtents = btVector3(props->shapeProps.halfExtentsForBox.x, props->shapeProps.halfExtentsForBox.y, props->shapeProps.halfExtentsForBox.z);
		shape = std::make_unique<btBoxShape>(halfExtents); //Shape is a smart pointer
		break;
	}
	case RigidBodyShape::None:
		return;
	default:
		return;
	}

	//Apply
	rigidBody->setCollisionShape(shape.get());

	btVector3 localInertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);

	if (!props->isKinematic) {
		shape->calculateLocalInertia(props->mass, localInertia);
		rigidBody->setMassProps(props->mass, localInertia);
	}
	else {
		rigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
	}
	rigidBody->updateInertiaTensor();
}
