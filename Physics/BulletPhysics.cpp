#include "pch.h"
#include "BulletPhysics.h"

void BulletPhysics::Init()
{
	// 1. Collision configuration
	collisionConfig = new btDefaultCollisionConfiguration();

	// 2. Dispatcher
	dispatcher = new btCollisionDispatcher(collisionConfig);

	// 3. Broadphase
	broadphase = new btDbvtBroadphase();

	// 4. Solver
	solver = new btSequentialImpulseConstraintSolver();

	// 5. Dynamics World
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

	// Gravity
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	/* STATIC PLANE IS NOT NECESSARY FOR NOW */
	// 6. Static Plane
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	//btRigidBody::btRigidBodyConstructionInfo
		//groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	//btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	//dynamicsWorld->addRigidBody(groundRigidBody);

	/* TODO: Move to entity manager */

	// 7. Dynamic Box
	
}

void BulletPhysics::Update(float dt)
{
	dynamicsWorld->stepSimulation(dt, 10);

	/*
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);

	printf("Box height: %f\n", trans.getOrigin().getY());*/
}

void BulletPhysics::Shutdown()
{
	//Remove rigidbodies
	for (auto iter = rigidbodies.begin(); iter != rigidbodies.end();++iter) {
		dynamicsWorld->removeRigidBody(iter->second->Get());
		iter->second->Delete();
	}

	rigidbodies.clear();

	//delete groundRigidBody->getMotionState();
	//delete groundRigidBody;
	//delete groundShape;

	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfig;
}

PHYSICS_API void BulletPhysics::AddRigidBody(std::string id)
{
	//Create a rigidbody object from RigidBody class and add it to Bullet physics
	RigidBody* rigidBody = new RigidBody();

	dynamicsWorld->addRigidBody(rigidBody->Create());

	rigidbodies.insert({ id, std::unique_ptr<RigidBody>(rigidBody)});
}

PHYSICS_API void BulletPhysics::RemoveRigidBody(std::string id)
{
	auto rigidBodyIter = rigidbodies.find(id);
	if (rigidBodyIter == rigidbodies.end())
		return;

	RigidBody* rigidBody = rigidBodyIter->second.get();

	dynamicsWorld->removeRigidBody(rigidBody->Get());

	rigidBody->Delete();

	delete rigidBody;

	rigidbodies.erase(rigidBodyIter);
}

PHYSICS_API void BulletPhysics::UpdateEntityTransforms(std::string id, glm::vec3& pos, glm::vec3& rot)
{
	//Find rigidbody from id
	auto iter = rigidbodies.find(id);

	if (iter == rigidbodies.end())
		return;

	btRigidBody* rb = iter->second->Get();

	//Get transfrom from rigidbody
	btTransform trans;
	rb->getMotionState()->getWorldTransform(trans);

	btVector3 position = trans.getOrigin();
	btQuaternion quat = trans.getRotation();

	btScalar yaw, pitch, roll;
	quat.getEulerZYX(yaw, pitch, roll);

	//Apply
	pos = glm::vec3(position.getX(), position.getY(), position.getZ());
	rot = glm::degrees(glm::vec3(roll, pitch, yaw));
}

PHYSICS_API void BulletPhysics::UpdateRigidbodyTransforms(std::string id, glm::vec3& pos, glm::vec3& rot)
{
	//Find rigidbody from id
	auto iter = rigidbodies.find(id);

	if (iter == rigidbodies.end())
		return;

	btRigidBody* rb = iter->second->Get();

	//Setup transform
	btTransform trans;
	
	btVector3 position;
	position.setValue(pos.x, pos.y, pos.z);
	trans.setOrigin(position);

	rot = glm::radians(rot);

	btQuaternion quat;
	quat.setEulerZYX(rot.z, rot.y, rot.x);
	trans.setRotation(quat);

	//Apply
	rb->setWorldTransform(trans);
	rb->getMotionState()->setWorldTransform(trans);
}

PHYSICS_API void BulletPhysics::StartEmulation()
{
	for (auto& rb : rigidbodies)
	{
		if (rb.second->GetProps()->isKinematic)
			continue;

		btRigidBody* body = rb.second->Get();

		// Clear kinematic flag
		int flags = body->getCollisionFlags();
		flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
		body->setCollisionFlags(flags);

		// Wake
		body->setActivationState(ACTIVE_TAG);
		body->activate(true);
	}
}

PHYSICS_API void BulletPhysics::EndEmulation()
{
	for (auto& rb : rigidbodies)
	{
		btRigidBody* body = rb.second->Get();

		int flags = body->getCollisionFlags();
		flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		body->setCollisionFlags(flags);

		body->setActivationState(DISABLE_DEACTIVATION);
	}
}

PHYSICS_API RigidBodyProperties* BulletPhysics::GetRigidBodyProperties(std::string id)
{
	//Find rigidbody from id
	auto iter = rigidbodies.find(id);

	if (iter == rigidbodies.end())
		return nullptr;

	//Return
	return iter->second->GetProps();
}

PHYSICS_API void BulletPhysics::SetRigidBodyProperties(std::string id, RigidBodyProperties* props)
{
	//Find rigidbody from id
	auto iter = rigidbodies.find(id);

	if (iter == rigidbodies.end())
		return;

	RigidBody* rigidBody = iter->second.get();

	//Set props
	rigidBody->SetProps(props);

	//Get rigidbody object of Bullet
	btRigidBody* btRb = rigidBody->Get();

	/* Apply changes */

	//Mass and inertia
	btVector3 inertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);
	//btRb->getCollisionShape()->calculateLocalInertia(props.mass, inertia);
	btRb->setMassProps(props->mass, inertia);

}
