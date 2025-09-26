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

#include <nlohmann_json/json.hpp>

//Stores properties for rigidbody
struct RigidBodyProperties {
	bool isKinematic = false;
	float mass = 1;
	glm::vec3 inertia = glm::vec3(0, 0, 0);
	float restitution = 0.5f;
	float linearDamping = 0.0f;
	float angularDamping = 0.0f;

	union ShapeProps {
		glm::vec3 halfExtentsForBox = glm::vec3(1.0f);
	} shapeProps;
};

enum class RigidBodyShape {
	Box, //Will be added more shapes
	None,
};

class RigidBody
{
public:
	PHYSICS_API btRigidBody* Create();
	PHYSICS_API void         Delete();
	PHYSICS_API btRigidBody* Get() { return rigidBody.get(); }
	PHYSICS_API RigidBodyShape GetShape();
	PHYSICS_API void           UpdateShapeHalfExtents(RigidBodyShape rbShape);

public:
	PHYSICS_API RigidBodyProperties* GetProps() { return props; };
	PHYSICS_API void				 SetProps(RigidBodyProperties* props) { this->props = props; } //Dont have to use it for now

public:
	//Serialization
	PHYSICS_API nlohmann::json ToJson();
	PHYSICS_API void FromJson(nlohmann::json& json);

private:
	std::shared_ptr<btRigidBody> rigidBody;
	std::shared_ptr<btCollisionShape> shape;
	btDefaultMotionState* motionState;

private:
	RigidBodyProperties* props = nullptr;
};

