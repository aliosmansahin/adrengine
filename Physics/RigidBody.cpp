#include "pch.h"
#include "RigidBody.h"

btRigidBody* RigidBody::Create()
{
	shape = std::make_shared<btBoxShape>(btVector3(1, 1, 1));

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

	props = new RigidBodyProperties();

	btVector3 inertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);
	shape->calculateLocalInertia(props->mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(props->mass, motionState, shape.get(), inertia);

	rigidBody = std::make_shared<btRigidBody>(fallRigidBodyCI);

	return rigidBody.get();
}

void RigidBody::Delete()
{
	if (motionState)
		delete motionState;
	if (props)
		delete props;
}
