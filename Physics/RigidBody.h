#pragma once

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#include <btBulletDynamicsCommon.h>
#include <memory>

class RigidBody
{
public:
	btRigidBody* Create();
	void         Delete();
	btRigidBody* Get() { return rigidBody.get(); }

public:
	bool isKinematic = false;

private:
	std::shared_ptr<btRigidBody> rigidBody;
	std::shared_ptr<btCollisionShape> shape;
	btDefaultMotionState* motionState;

private:
	//Dynamics
	btScalar mass = 1;
	btVector3 inertia = btVector3(0, 0, 0);
};

