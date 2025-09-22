#pragma once

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#include <btBulletDynamicsCommon.h>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


//Stores properties for rigidbody
struct RigidBodyProperties {
	bool isKinematic = false;
	float mass = 1;
	glm::vec3 inertia = glm::vec3(3, 0, 0);
};

class RigidBody
{
public:
	btRigidBody* Create();
	void         Delete();
	btRigidBody* Get() { return rigidBody.get(); }

public:
	RigidBodyProperties* GetProps() { return props; };
	void				 SetProps(RigidBodyProperties* props) { this->props = props; } //Dont have to use it for now

private:
	std::shared_ptr<btRigidBody> rigidBody;
	std::shared_ptr<btCollisionShape> shape;
	btDefaultMotionState* motionState;

private:
	RigidBodyProperties* props = nullptr;
};

