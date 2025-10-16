#pragma once

#include "../ILight.h"
#include "ISpotLightParams.h"

class ISpotLight : public virtual ILight
{
public:
	virtual ~ISpotLight() = default;

	virtual glm::mat4 GetLightSpaceMatrix() = 0;
};

