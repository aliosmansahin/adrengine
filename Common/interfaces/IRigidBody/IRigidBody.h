#pragma once

#include "../../nlohmann_json/json.hpp"
#include <glm/glm.hpp>

class btRigidBody;

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

class IRigidBody
{
public:
	virtual ~IRigidBody() = default;

public:
	virtual std::shared_ptr<btRigidBody> Create() = 0;
	virtual void					     Delete() = 0;
	virtual std::shared_ptr<btRigidBody> Get() = 0;
	virtual RigidBodyShape GetShape() = 0;
	virtual void           UpdateShapeHalfExtents(RigidBodyShape rbShape) = 0;

public:
	virtual std::shared_ptr<RigidBodyProperties> GetProps() = 0;
	virtual void				 SetProps(std::shared_ptr<RigidBodyProperties> props) = 0; //Dont have to use it for now

public:
	//Serialization
	virtual nlohmann::json ToJson() = 0;
	virtual void FromJson(nlohmann::json& json) = 0;
};

