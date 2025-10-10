#pragma once

#include "..\IEntity.h"
#include "../IObject/IObjectParams.h"

#include "../../IRigidBody/IRigidBody.h"

class IObject : public virtual IEntity
{
public:
	virtual void AddImpulse(glm::vec3 impulse) = 0;

	virtual void ResetPhysics() = 0;

	virtual glm::mat4 GetModelMatrix() = 0;

	virtual std::shared_ptr<IRigidBody>& GetRigidBody() = 0;
	virtual void SetRigidBody(std::shared_ptr<IRigidBody> rigidBody) = 0;
};
