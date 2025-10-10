#pragma once

#include "../ILight.h"
#include "IPointLightParams.h"

class IPointLight : public virtual ILight
{
public:
	virtual float GetFarPlane() = 0;
};

