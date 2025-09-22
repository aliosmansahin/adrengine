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
	//delete groundRigidBody->getMotionState();
	//delete groundRigidBody;
	//delete groundShape;

	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfig;
}

PHYSICS_API void BulletPhysics::AddRigidBody(RigidBody*& rigidBody)
{
	//Create a rigidbody object from RigidBody class and add it to Bullet physics
	rigidBody = new RigidBody();

	dynamicsWorld->addRigidBody(rigidBody->Create());
}

PHYSICS_API void BulletPhysics::RemoveRigidBody(RigidBody*& rigidBody)
{
	dynamicsWorld->removeRigidBody(rigidBody->Get());

	rigidBody->Delete();

	delete rigidBody;
}

PHYSICS_API void BulletPhysics::UpdateEntityTransforms(RigidBody* rigidBody, glm::vec3& pos, glm::vec3& rot)
{
	btRigidBody* rb = rigidBody->Get();

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

PHYSICS_API void BulletPhysics::UpdateRigidbodyTransforms(RigidBody* rigidBody, glm::vec3& pos, glm::vec3& rot)
{
	btRigidBody* rb = rigidBody->Get();

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

PHYSICS_API void BulletPhysics::StartEmulationForRigidBody(RigidBody* rigidBody)
{
	if (rigidBody->GetProps()->isKinematic)
		return;

	btRigidBody* body = rigidBody->Get();

	// Clear kinematic flag
	int flags = body->getCollisionFlags();
	flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
	body->setCollisionFlags(flags);

	// Wake
	body->setActivationState(ACTIVE_TAG);
	body->activate(true);
}

PHYSICS_API void BulletPhysics::EndEmulationForRigidBody(RigidBody* rigidBody)
{
	btRigidBody* body = rigidBody->Get();

	int flags = body->getCollisionFlags();
	flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
	body->setCollisionFlags(flags);

	body->setActivationState(DISABLE_DEACTIVATION);
}

PHYSICS_API void BulletPhysics::ApplyPropsForRigidBody(RigidBody* rigidBody)
{
	//Get props
	RigidBodyProperties* props = rigidBody->GetProps();

	//Get rigidbody object of Bullet
	btRigidBody* btRb = rigidBody->Get();

	/* Apply changes */

	//Mass and inertia
	btVector3 inertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);
	//btRb->getCollisionShape()->calculateLocalInertia(props.mass, inertia);
	btRb->setMassProps(props->mass, inertia);
}
