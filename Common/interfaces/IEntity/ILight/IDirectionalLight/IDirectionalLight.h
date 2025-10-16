#pragma once

#include "../ILight.h"
#include "IDirectionalLightParams.h"

class IDirectionalLight : public virtual ILight
{
public:
	virtual ~IDirectionalLight() = default;

	virtual glm::mat4 GetLightSpaceMatrix() = 0;
};

