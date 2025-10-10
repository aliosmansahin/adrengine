#pragma once

#include "../ILight.h"
#include "IDirectionalLightParams.h"

class IDirectionalLight : public virtual ILight
{
public:
	virtual glm::mat4 GetLightSpaceMatrix() = 0;
};

