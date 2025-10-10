#include "pch.h"
#include "BulletPhysics.h"

/*

PURPOSE: Initializes Bullet Physics: Sets config, dynamicWorld etc...

*/
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

/*

PURPOSE: Updates physics

*/
void BulletPhysics::Update(float dt)
{
	dynamicsWorld->stepSimulation(dt, 10);
}


/*

PURPOSE: Clears physics

*/
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

/*

PURPOSE: Creates a new rigidbody into out parameter and pass it into dynamicsWorld (physics)

*/
PHYSICS_API void BulletPhysics::AddRigidBody(std::shared_ptr<IRigidBody>& rigidBody)
{
	//Create a rigidbody object from RigidBody class and add it to Bullet physics
	rigidBody = std::make_shared<RigidBody>();

	dynamicsWorld->addRigidBody(rigidBody->Create().get());
}

/*

PURPPOSE: Removes the rigidbody that passed from the parameter from the dynamicsWorld then deletes it

*/
PHYSICS_API void BulletPhysics::RemoveRigidBody(std::shared_ptr<IRigidBody>& rigidBody)
{
	dynamicsWorld->removeRigidBody(rigidBody->Get().get());

	rigidBody->Delete();
}

/*

PURPOSE: Updates transform of an entity(object) from its rigidbody

*/
PHYSICS_API void BulletPhysics::UpdateEntityTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot)
{
	std::shared_ptr<btRigidBody> rb = rigidBody->Get();

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


/*

PURPOSE: Updates transform of a rigidbody from its entity(object)

*/
PHYSICS_API void BulletPhysics::UpdateRigidbodyTransforms(std::shared_ptr<IRigidBody> rigidBody, glm::vec3& pos, glm::vec3& rot)
{
	std::shared_ptr<btRigidBody> rb = rigidBody->Get();

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

/*

PURPOSE: Makes rigidbody ready to simulate physics

*/
PHYSICS_API void BulletPhysics::StartEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody)
{
	std::shared_ptr<btRigidBody> body = rigidBody->Get();

	if (!rigidBody->GetProps()->isKinematic) {
		// Clear kinematic flag
		int flags = body->getCollisionFlags();
		flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
		body->setCollisionFlags(flags);
	}

	// Apply Props
	ApplyPropsForRigidBody(rigidBody);

	// Wake
	body->activate(true);
}

/*

PURPOSE: Makes rigidbody deactivate to simulate physics

*/
PHYSICS_API void BulletPhysics::EndEmulationForRigidBody(std::shared_ptr<IRigidBody> rigidBody)
{
	std::shared_ptr<btRigidBody> body = rigidBody->Get();

	int flags = body->getCollisionFlags();
	flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
	body->setCollisionFlags(flags);
}

/*

PURPOSE: Applies all properties of rigidbody

*/
PHYSICS_API void BulletPhysics::ApplyPropsForRigidBody(std::shared_ptr<IRigidBody> rigidBody)
{
	//Get props
	std::shared_ptr<RigidBodyProperties> props = rigidBody->GetProps();

	//Get rigidbody object of Bullet
	std::shared_ptr<btRigidBody> btRb = rigidBody->Get();

	/* Remove rigidbody to update */
	dynamicsWorld->removeRigidBody(btRb.get());

	/* Apply changes */

	//Mass and inertia
	btVector3 inertia = btVector3(props->inertia.x, props->inertia.y, props->inertia.z);
	btRb->setMassProps(props->mass, inertia);

	RigidBodyShape rbShape = rigidBody->GetShape();
	
	//Extents
	rigidBody->UpdateShapeHalfExtents(rbShape);

	//Restitution
	btRb->setRestitution(props->restitution);

	//Damping
	btRb->setDamping(props->linearDamping, props->angularDamping);

	/* Readd rigidbody */
	dynamicsWorld->addRigidBody(btRb.get());
	dynamicsWorld->updateSingleAabb(btRb.get());

	btRb->activate(true);
}
