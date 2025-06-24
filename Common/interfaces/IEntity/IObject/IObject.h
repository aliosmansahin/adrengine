#pragma once

#include "..\IEntity.h"

class IObject : public IEntity
{
public:
	virtual void AddImpulse(glm::vec3 impulse) = 0;
};

