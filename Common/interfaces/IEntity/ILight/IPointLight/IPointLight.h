#pragma once

#include "../ILight.h"
#include "IPointLightParams.h"

class IPointLight : public virtual ILight
{
public:
	virtual ~IPointLight() = default;

	virtual float GetFarPlane() = 0;
};

