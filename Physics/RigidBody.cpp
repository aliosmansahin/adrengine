#include "pch.h"
#include "RigidBody.h"

/*

PURPOSE: Creates a RigidBody object and returns it

*/
btRigidBody* RigidBody::Create()
{
	//Setup rigidbody
	shape = std::make_shared<btBoxShape>(btVector3(1, 1, 1));

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

	props = new RigidBodyProperties();
	props->shapeProps.halfExtentsForBox = glm::vec3(1.0f);

	btVector3 inertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);
	shape->calculateLocalInertia(props->mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(props->mass, motionState, shape.get(), inertia);

	rigidBody = std::make_shared<btRigidBody>(fallRigidBodyCI);

	rigidBody->setRestitution(props->restitution);
	rigidBody->setDamping(props->linearDamping, props->angularDamping);
	
	rigidBody->setActivationState(DISABLE_DEACTIVATION);

	int flags = rigidBody->getCollisionFlags();
	flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
	rigidBody->setCollisionFlags(flags);

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

/*

PURPOSE: Creates a json content from the rigidbody

*/
nlohmann::json RigidBody::ToJson() {
	nlohmann::json j;

	RigidBodyShape rbShape = GetShape();

	//Save shape
	j["shape"] = rbShape;

	//Save props
	j["is-kinematic"] = props->isKinematic;
	j["mass"] = props->mass;
	j["inertia-x"] = props->inertia.x;
	j["inertia-y"] = props->inertia.y;
	j["inertia-z"] = props->inertia.z;
	j["restitution"] = props->restitution;
	j["linear-damping"] = props->linearDamping;
	j["angular-damping"] = props->angularDamping;

	//Shape specified
	switch (rbShape)
	{
	case RigidBodyShape::Box:
		j["box-half-extents-x"] = props->shapeProps.halfExtentsForBox.x;
		j["box-half-extents-y"] = props->shapeProps.halfExtentsForBox.y;
		j["box-half-extents-z"] = props->shapeProps.halfExtentsForBox.z;
		break;
	default:
		break;
	}

	return j;
}

void RigidBody::FromJson(nlohmann::json& json) {
	//Get shape
	RigidBodyShape rbShape = json.value("shape", RigidBodyShape::None);

	//Get props
	props->isKinematic = json.value("is-kinematic", true);
	props->mass = json.value("mass", 1.0f);

	props->inertia.x = json.value("inertia-x", 0.0f);
	props->inertia.y = json.value("inertia-y", 0.0f);
	props->inertia.z = json.value("inertia-z", 0.0f);

	props->restitution = json.value("restitution", 0.5f);
	props->linearDamping = json.value("linear-damping", 0.0f);
	props->angularDamping = json.value("angular-damping", 0.0f);

	//Get shape specific
	switch (rbShape)
	{
	case RigidBodyShape::Box:
		props->shapeProps.halfExtentsForBox.x = json.value("box-half-extents-x", 0.0f);
		props->shapeProps.halfExtentsForBox.y = json.value("box-half-extents-y", 0.0f);
		props->shapeProps.halfExtentsForBox.z = json.value("box-half-extents-z", 0.0f);
		break;
	default:
		break;
	}
}