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

#include "interfaces/IRigidBody/IRigidBody.h"

class RigidBody : public IRigidBody
{
public:
	PHYSICS_API std::shared_ptr<btRigidBody> Create() override;
	PHYSICS_API void						 Delete() override;
	PHYSICS_API std::shared_ptr<btRigidBody> Get() override { return rigidBody; }
	PHYSICS_API RigidBodyShape GetShape() override;
	PHYSICS_API void           UpdateShapeHalfExtents(RigidBodyShape rbShape) override;

public:
	PHYSICS_API std::shared_ptr<RigidBodyProperties> GetProps() override { return props; };
	PHYSICS_API void				 SetProps(std::shared_ptr<RigidBodyProperties> props) override { this->props = props; } //Dont have to use it for now

public:
	//Serialization
	PHYSICS_API nlohmann::json ToJson() override;
	PHYSICS_API void FromJson(nlohmann::json& json) override;

private:
	std::shared_ptr<btRigidBody> rigidBody;
	std::shared_ptr<btCollisionShape> shape;
	std::shared_ptr<btDefaultMotionState> motionState;

private:
	std::shared_ptr<RigidBodyProperties> props = nullptr;
};

